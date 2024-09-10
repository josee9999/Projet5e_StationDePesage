#define _GNU_SOURCE

// INCLUSIONS
#include <stdio.h>
#include <fcntl.h>   // File Control Definitions
#include <termios.h> // POSIX Terminal Control Definitions
#include <unistd.h>  // UNIX Standard Definitions
#include <errno.h>
#include <sys/types.h> // Pour pid_t
#include <sys/wait.h>  // Pour wait()
#include <stdlib.h>    // Pour exit()
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <math.h>

#include "piloteSerieUSB_Bras.h"
#include "piloteSerieUSB_Balance.h"
#include "piloteI2C.h"
#include "interfaceUArm.h"
#include "interfaceBalance.h"
#include "interfaceVL6180x.h"
#include "processusBras.h"

#include "piloteSerieUSB_Bras.c"
#include "piloteSerieUSB_Balance.c"
#include "piloteI2C.c"
#include "interfaceUArm.c"
#include "interfaceBalance.c"
#include "interfaceVL6180x.c"
#include "processusBras.c"

// Definitions privees
// pas de definitions privees

// Declarations de fonctions privees:
int main_initialise(void);
char cAnalyserMessageCAN(struct can_frame *frame);
void PreparerTrameCAN(struct can_frame *frame, char cOct0, char cOct1, char cOct2, char cOct3, char cOct4, char cOct5, char cOct6, char cOct7);
void main_termine(void);

// Definitions de variables privees:
// pas de variables privees

// Definitions de fonctions privees:
int main_initialise(void)
{
  if (piloteSerieUSB_Bras_initialise() != 0)
  {
    return 3;
  }
  if (piloteI2C_initialise() != 0)
  {
    return 3;
  }
  if (piloteSerieUSB_Balance_initialise() != 0)
  {
    return 3;
  }
  if (interfaceUArm_initialise() != 0)
  {
    return 3;
  }
  if (interfaceVL6810x_initialise() != 0)
  {
    return 3;
  }
  return 0;
}

char cAnalyserMessageCAN(struct can_frame *frame)
{
  char cMessage;

  if ((frame->data[0] == 0x05) && (frame->data[1] == 'I') && (frame->data[5] == 0x01))
  {
    cMessage = 'v';
  }
  else if ((frame->data[0] == 0x02) && (frame->data[1] == 'I') && (frame->data[2] == 'O'))
  {
    cMessage = 'o';
  }
  else if ((frame->data[0] == 0x02) && (frame->data[1] == 'I') && (frame->data[2] == 'M'))
  {
    cMessage = 'm';
  }
  else if ((frame->data[1] == 'A'))
  {
    cMessage = 'a';
  }
  else if ((frame->data[1] == 'D'))
  {
    cMessage = 'd';
  }
  else if ((frame->data[1] == 'R'))
  {
    cMessage = 'r';
  }
  return cMessage;
}

void main_termine(void)
{
  piloteSerieUSB_Bras_termine();
  piloteSerieUSB_Balance_termine();
  interfaceUArm_termine();
}

// Definitions de variables publiques:
// pas de variables publiques

// Definitions de fonctions publiques:
int main(int argc, char **argv)
{
  int pipeBras[2], pipeBalance[2], pipeCAN[2];
  pid_t pidBras, pidBalance, pidCAN;
  int fdSocketCAN;
  int nbytes;
  struct sockaddr_can addr;
  struct ifreq ifr;
  struct can_frame frame;
  char cMessage = 'z';
  char cCouleur = '0';
  char cFlagBras = 0;

  pipe(pipeBras);
  pipe(pipeBalance);
  pipe(pipeCAN);

  if (main_initialise())
  {
    printf("main_initialise: erreur\n");
    return 0;
  }

  // Create and bind the CAN socket in the parent process
  if ((fdSocketCAN = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
  {
    perror("Socket");
    return -1;
  }

  strcpy(ifr.ifr_name, "can0");
  ioctl(fdSocketCAN, SIOCGIFINDEX, &ifr);

  memset(&addr, 0, sizeof(addr));
  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;

  if (bind(fdSocketCAN, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    perror("Bind");
    return -1;
  }

  // Fork pour le processus bras (enfant #1)
  if ((pidBras = fork()) == 0)
  {
    close(pipeBras[1]);
    char buffer[32];
    while (1)
    {
      if (read(pipeBras[0], buffer, 32) > 0)
      {
        if (strcmp(buffer, "d") == 0)
        {
          cFlagBras = 1;
        }
        if (cFlagBras == 1)
        {
          if (strcmp(buffer, "a") == 0)
          {
            cFlagBras = 0;
          }
          if (strcmp(buffer, "r") == 0)
          {
            processusBras_RetourBase();
            printf("Reset\n");
            cFlagBras = 0;
          }
          if (strcmp(buffer, "o") == 0)
          {
            processusBras_TrouvePoid('o');
            processusBras_PrendrePoid();

            PreparerTrameCAN(&frame, 0x04, 'C', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
            if (write(fdSocketCAN, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
            {
              perror("Write orange");
              return -1;
            }

            processusBras_DiscarterOrange();
            processusBras_RetourBase();
            PreparerTrameCAN(&frame, 0x04, 'F', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
            if (write(fdSocketCAN, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
            {
              perror("Write fin orange");
              return -1;
            }
            printf("F orange\n");
          }
          else if (strcmp(buffer, "m") == 0)
          {
            processusBras_TrouvePoid('m');
            processusBras_PrendrePoid();

            PreparerTrameCAN(&frame, 0x04, 'C', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
            if (write(fdSocketCAN, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
            {
              perror("Write metal");
              return -1;
            }

            processusBras_AllerBalance();
          }
          else if (strcmp(buffer, "ApresBalance") == 0)
          {
            processusBras_DiscarterMetal();
            processusBras_RetourBase();

            PreparerTrameCAN(&frame, 0x04, 'F', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
            if (write(fdSocketCAN, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
            {
              perror("Write fin metal");
              return -1;
            }
            printf("F metal\n");
          }
        }
      }
    }
    close(pipeBras[0]);
    exit(0);
  }

  // Fork pour le processus balance (enfant #2)
  if ((pidBalance = fork()) == 0)
  {
    float fPoidsEnfant = -1;
    close(pipeBalance[0]);
    while (1)
    {
      fPoidsEnfant = -1;

      fPoidsEnfant = interfaceBalance_LecturePoids(piloteSerieUSB_Balance_fichier);
      fPoidsEnfant = interfaceBalance_ValiderValeur(fPoidsEnfant);

      if (fPoidsEnfant != -1)
      {
        write(pipeBalance[1], &fPoidsEnfant, sizeof(fPoidsEnfant));
      }
    }
    close(pipeBalance[1]);
    exit(0);
  }

  // Fork pour le processus CAN (enfant #3)
  if ((pidCAN = fork()) == 0)
  {
    close(pipeCAN[0]);
    char bufferMessageCAN[32];

    while (1)
    {
      nbytes = read(fdSocketCAN, &frame, sizeof(struct can_frame));

      if (nbytes < 0)
      {
        perror("Read");
        return -1;
      }

      if (write(pipeCAN[1], &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
      {
        perror("Write to pipe");
        return -1;
      }
    }
    close(pipeCAN[1]);
    exit(0);
  }

  // Processus parent
  close(pipeBras[0]);
  close(pipeBalance[1]);
  close(pipeCAN[1]);

  while (1)
  {
    cMessage = 'z';
    nbytes = read(pipeCAN[0], &frame, sizeof(struct can_frame));

    if (nbytes < 0)
    {
      perror("Read from pipe");
      return -1;
    }

    cMessage = cAnalyserMessageCAN(&frame);
    switch (cMessage)
    {
    case 'a':
      write(pipeBras[1], "a", strlen("a") + 1);
      printf("case 'a'\n");
      break;
    case 'd':
      write(pipeBras[1], "d", strlen("d") + 1);
      printf("case 'd'\n");
      break;
    case 'r':
      write(pipeBras[1], "r", strlen("r") + 1);
      printf("case 'r'\n");
      break;
    case 'o':
      cCouleur = 'o';
      printf("case 'o'\n");
      break;
    case 'm':
      cCouleur = 'm';
      printf("case 'm'\n");
      break;
    case 'v':
      printf("case 'v'\n");
      if (cCouleur == 'o')
      {
        write(pipeBras[1], "o", strlen("o") + 1);
      }
      else if (cCouleur == 'm')
      {
        write(pipeBras[1], "m", strlen("m") + 1);

        float fPoidsParent = -1;
        char cNbAvantPoint;
        char cNbApresPoint;
        do
        {
          read(pipeBalance[0], &fPoidsParent, sizeof(fPoidsParent));
        } while ((fPoidsParent < 30) || (fPoidsParent > 100));
      
        if(fPoidsParent <= 76.60)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x60;
        }
        else if(fPoidsParent <= 76.61)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x61;
        }else if(fPoidsParent <= 76.62)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x62;
        }else if(fPoidsParent <= 76.63)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x63;
        }else if(fPoidsParent <= 76.64)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x64;
        }else if(fPoidsParent <= 76.65)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x65;
        }else if(fPoidsParent <= 76.66)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x66;
        }else if(fPoidsParent <= 76.67)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x67;
        }else if(fPoidsParent <= 76.68)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x68;
        }else if(fPoidsParent <= 76.69)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x69;
        }else if(fPoidsParent <= 76.70)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x70;
        }else if(fPoidsParent <= 76.71)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x71;
        }else if(fPoidsParent <= 76.72)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x72;
        }else if(fPoidsParent <= 76.73)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x73;
        }else if(fPoidsParent <= 76.74)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x74;
        }else if(fPoidsParent <= 76.75)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x75;
        }else if(fPoidsParent <= 76.76)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x76;
        }else if(fPoidsParent <= 76.77)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x77;
        }else if(fPoidsParent <= 76.78)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x78;
        }else if(fPoidsParent <= 76.79)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x79;
        }else if(fPoidsParent <= 76.80)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x80;
        }else if(fPoidsParent <= 76.81)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x81;
        }else if(fPoidsParent <= 76.82)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x82;
        }else if(fPoidsParent <= 76.83)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x83;
        }else if(fPoidsParent <= 76.84)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x84;
        }else if(fPoidsParent <= 76.85)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x85;
        }else if(fPoidsParent <= 76.86)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x86;
        }else if(fPoidsParent <= 76.87)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x87;
        }else if(fPoidsParent <= 76.88)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x88;
        }else if(fPoidsParent <= 76.89)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x89;
        }else if(fPoidsParent <= 76.90)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x90;
        }else if(fPoidsParent <= 76.91)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x91;
        }else if(fPoidsParent <= 76.92)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x92;
        }else if(fPoidsParent <= 76.93)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x93;
        }else if(fPoidsParent <= 76.94)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x94;
        }else if(fPoidsParent <= 76.95)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x95;
        }else if(fPoidsParent <= 76.96)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x96;
        }else if(fPoidsParent <= 76.97)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x97;
        }else if(fPoidsParent <= 76.98)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x98;
        }else if(fPoidsParent <= 76.99)
        { 
          cNbAvantPoint = 0x76;
          cNbApresPoint = 0x99;
        }

        printf("cNbAvantPoint: %c\n", cNbAvantPoint);
        printf("cNbApresPoint: %c\n", cNbApresPoint);

        PreparerTrameCAN(&frame, 0x04, 'I', 'M', cNbAvantPoint, cNbApresPoint, 0x00, 0x00, 0x00);
        if (write(fdSocketCAN, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        {
          perror("Write poids");
          return -1;
        }

        write(pipeBras[1], "ApresBalance", strlen("ApresBalance") + 1);
      }
    default:
      printf("Valeur non pris en charge (char :%c - hex %x)\n", cMessage, cMessage); 
    }
  }

  close(pipeBras[1]);
  close(pipeBalance[0]);
  close(pipeCAN[0]);
  close(fdSocketCAN);

  kill(pidBras, SIGKILL);
  kill(pidBalance, SIGKILL);
  kill(pidCAN, SIGKILL);

  wait(NULL);
  wait(NULL);
  wait(NULL);

  main_termine();
  return EXIT_SUCCESS;
}

void PreparerTrameCAN(struct can_frame *frame, char cOct0, char cOct1, char cOct2, char cOct3, char cOct4, char cOct5, char cOct6, char cOct7)
{
  frame->can_id = 0x190; // identifiant CAN
  frame->can_dlc = 8;    // nombre d'octets de données

  frame->data[0] = cOct0;
  frame->data[1] = cOct1;
  frame->data[2] = cOct2;
  frame->data[3] = cOct3;
  frame->data[4] = cOct4;
  frame->data[5] = cOct5;
  frame->data[6] = cOct6;
  frame->data[7] = cOct7;
}