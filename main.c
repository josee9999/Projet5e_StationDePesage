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
    return -1;
  }
  if (piloteI2C_initialise() != 0)
  {
    return -1;
  }
  if (piloteSerieUSB_Balance_initialise() != 0)
  {
    return -1;
  }
  if (interfaceUArm_initialise() != 0)
  {
    return -1;
  }
  if (interfaceVL6810x_initialise() != 0)
  {
    return -1;
  }
  return 0;
}

char cAnalyserMessageCAN(struct can_frame *frame)
{
  char cMessage;

  if ((frame->data[0] == '5') && (frame->data[1] == 'I') && (frame->data[5] == '1'))
  {
    cMessage = 'v';
  }
  else if ((frame->data[0] == '2') && (frame->data[1] == 'I') && (frame->data[2] == 'O'))
  {
    cMessage = 'o';
  }
  else if ((frame->data[0] == '2') && (frame->data[1] == 'I') && (frame->data[2] == 'M'))
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
    cMessage = 'a';
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
  char cMessage = '0';
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
            printf("RESET"); // cas reset a faire
            cFlagBras = 0;
          }
          if (strcmp(buffer, "o") == 0)
          {
            processusBras_TrouvePoid('o');
            // processusBras_PrendrePoid('o');
          }
          else if (strcmp(buffer, "m") == 0)
          {
            processusBras_TrouvePoid('m');
            // processusBras_PrendrePoid('m');
          }
          else if (strcmp(buffer, "ApresBalance") == 0)
          {
            processusBras_DiscarterMetal();
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
  }

  // Processus parent
  close(pipeBras[0]);
  close(pipeBalance[1]);
  close(pipeCAN[1]);

  while (1)
  {
    cMessage = '0';
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
        cNbAvantPoint = '0' + (int)fPoidsParent;
        cNbApresPoint = '0' + ((int)((fabs(fPoidsParent - cNbAvantPoint) * 10)));

        PreparerTrameCAN(&frame, '4', 'I', 'M', cNbAvantPoint, cNbApresPoint, '0', '0', '0');
        if (write(fdSocketCAN, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        {
          perror("Write");
          return -1;
        }

        write(pipeBras[1], "ApresBalance", strlen("ApresBalance") + 1);
      }
    default:
      printf("ERREUR"); // cas erreur a faire
    }
  }

  close(pipeBras[1]);
  close(pipeBalance[0]);
  close(pipeCAN[0]);

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
  frame->can_id = 0x190; // identifiant CAN, exemple: 247 = 0x0F7
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