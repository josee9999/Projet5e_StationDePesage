//piloteSerieUSB
//Historique: 
//2018-11-12, Yves Roy, creation


#define _GNU_SOURCE
//INCLUSIONS
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "main.h"
#include "piloteSerieUSB_Bras.h"

//Definitions privees
#define PILOTESERIEUSB_TTY0 "/dev/ttyACM0"
#define PILOTESERIEUSB_TTY1 "/dev/ttyACM1"
#define PILOTESERIEUSB_TTY2 "/dev/ttyACM2"
#define PILOTESERIEUSB_TTY3 "/dev/ttyACM3"
#define PILOTESERIEUSB_TTY4 "/dev/ttyACM4"
#define PILOTESERIEUSB_TTY5 "/dev/ttyACM5"
#define PILOTESERIEUSB_TTY6 "/dev/ttyACM6"
#define PILOTESERIEUSB_TTY7 "/dev/ttyACM7"
#define PILOTESERIEUSB_TTY8 "/dev/ttyACM8"
#define PILOTESERIEUSB_TTY9 "/dev/ttyACM9"
#define PILOTESERIEUSB_TTY10 "/dev/ttyACM10"

//Declarations de fonctions privees:
//pas de fonctions privees

//Definitions de variables privees:
int piloteSerieUSB_Bras_fichier;
struct termios piloteSerieUSB_Bras_configuration;
int fd_portUART_Bras;
//Definitions de fonctions privees:
//pas de fonctions privees

//Definitions de variables publiques:
//pas de variables publiques

//Definitions de fonctions publiques:
int piloteSerieUSB_Bras_initialise(void)
{
  // Ouverture du port série 
  piloteSerieUSB_Bras_fichier = open(PILOTESERIEUSB_TTY0, O_RDWR | O_NOCTTY | O_NDELAY);
  if (piloteSerieUSB_Bras_fichier == -1)
  {
    piloteSerieUSB_Bras_fichier = open(PILOTESERIEUSB_TTY1, O_RDWR | O_NOCTTY | O_NDELAY);    
    if (piloteSerieUSB_Bras_fichier == -1)
    {
      piloteSerieUSB_Bras_fichier = open(PILOTESERIEUSB_TTY2, O_RDWR | O_NOCTTY | O_NDELAY);    
      if (piloteSerieUSB_Bras_fichier == -1)
      {
        piloteSerieUSB_Bras_fichier = open(PILOTESERIEUSB_TTY3, O_RDWR | O_NOCTTY | O_NDELAY);    
        if (piloteSerieUSB_Bras_fichier == -1)
        {
          piloteSerieUSB_Bras_fichier = open(PILOTESERIEUSB_TTY4, O_RDWR | O_NOCTTY | O_NDELAY);    
          if (piloteSerieUSB_Bras_fichier == -1)
          {
            piloteSerieUSB_Bras_fichier = open(PILOTESERIEUSB_TTY5, O_RDWR | O_NOCTTY | O_NDELAY);    
            if (piloteSerieUSB_Bras_fichier == -1)
            {
              piloteSerieUSB_Bras_fichier = open(PILOTESERIEUSB_TTY6, O_RDWR | O_NOCTTY | O_NDELAY);    
              if (piloteSerieUSB_Bras_fichier == -1)
              {
                piloteSerieUSB_Bras_fichier = open(PILOTESERIEUSB_TTY7, O_RDWR | O_NOCTTY | O_NDELAY);
                if (piloteSerieUSB_Bras_fichier == -1)
                {
                  piloteSerieUSB_Bras_fichier = open(PILOTESERIEUSB_TTY8, O_RDWR | O_NOCTTY | O_NDELAY);
                  if (piloteSerieUSB_Bras_fichier == -1)
                  {
                    piloteSerieUSB_Bras_fichier = open(PILOTESERIEUSB_TTY9, O_RDWR | O_NOCTTY | O_NDELAY);
                    if (piloteSerieUSB_Bras_fichier == -1)
                    {
                      piloteSerieUSB_Bras_fichier = open(PILOTESERIEUSB_TTY10, O_RDWR | O_NOCTTY | O_NDELAY);
                      if (piloteSerieUSB_Bras_fichier == -1)
                      {
                        perror("Erreur! Ouverture de port (Bras)");
                        return EXIT_FAILURE;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  struct termios SerialPortSettings; // Create the structure 
  tcgetattr(piloteSerieUSB_Bras_fichier, &SerialPortSettings); // Get the current attributes of the Serial port

  // Setting the Baud rate
  cfsetispeed(&SerialPortSettings, PILOTESERIEUSB_BRAS_BAUDRATE_AVEC_B_AU_DEBUT); // Set Read Speed   
  cfsetospeed(&SerialPortSettings, PILOTESERIEUSB_BRAS_BAUDRATE_AVEC_B_AU_DEBUT); // Set Write Speed  
 
  //8N1
  SerialPortSettings.c_cflag &= ~PARENB; // No Parity
  SerialPortSettings.c_cflag &= ~CSTOPB; // 1 Stop bit
  SerialPortSettings.c_cflag &= ~CSIZE;  // Clears the mask for setting the data size
  SerialPortSettings.c_cflag |= CS8;      // Set the data bits = 8
  SerialPortSettings.c_cflag &= ~CRTSCTS; // No Hardware flow Control
  SerialPortSettings.c_cflag |= CREAD | CLOCAL; // Enable receiver, Ignore Modem Control lines

  SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable XON/XOFF flow control

  SerialPortSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //  Non Cannonical mode, Disable echo, Disable signal  
  SerialPortSettings.c_oflag &= ~OPOST; // No Output Processing

  // Setting Time outs 
  SerialPortSettings.c_cc[VMIN] = 19; // Read at least 0 character(s) 
  SerialPortSettings.c_cc[VTIME] = 0; // Wait 10 sec (0 for indefinitely) 

  // Set the attributes to the termios structure
  if (tcsetattr(piloteSerieUSB_Bras_fichier, TCSANOW, &SerialPortSettings) != 0) 
  {
    perror("Erreur! configuration des attributs du port serie");
    exit(EXIT_FAILURE);
  }

  return 0;
}

int piloteSerieUSB_Bras_termine(void)
{
  
  tcflush(piloteSerieUSB_Bras_fichier, TCIOFLUSH);   
  close(piloteSerieUSB_Bras_fichier);
  return 0;
}



int piloteSerieUSB_Bras_ecrit(char *Source, unsigned char NombreATransmettre)
{
  sleep(1);
  tcflush(piloteSerieUSB_Bras_fichier, TCOFLUSH);  
  sleep(1);//ajout pour test
  return (int)write(piloteSerieUSB_Bras_fichier,Source, NombreATransmettre);
}

int piloteSerieUSB_Bras_attendLaFinDeLEcriture(void)
{
  return (int)tcdrain(piloteSerieUSB_Bras_fichier);
}

int piloteSerieUSB_Bras_lit(char *Destination, unsigned char NombreMaximalDeLectures)
{
  int retour = read(piloteSerieUSB_Bras_fichier, Destination, NombreMaximalDeLectures);  
  tcflush(piloteSerieUSB_Bras_fichier, TCIFLUSH);  
  return retour;
}