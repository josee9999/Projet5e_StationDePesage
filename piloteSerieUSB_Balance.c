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
#include "piloteSerieUSB_Balance.h"

//Definitions privees
#define PILOTESERIEUSB_USB0 "/dev/ttyUSB0"
#define PILOTESERIEUSB_USB1 "/dev/ttyUSB1"
#define PILOTESERIEUSB_USB2 "/dev/ttyUSB2"
#define PILOTESERIEUSB_USB3 "/dev/ttyUSB3"
#define PILOTESERIEUSB_USB4 "/dev/ttyUSB4"
#define PILOTESERIEUSB_USB5 "/dev/ttyUSB5"
#define PILOTESERIEUSB_USB6 "/dev/ttyUSB6"

//Declarations de fonctions privees:
//pas de fonctions privees

//Definitions de variables privees:

struct termios piloteSerieUSB_Balance_configuration;
int fd_portUART_Balance;
//Definitions de fonctions privees:
//pas de fonctions privees

//Definitions de variables publiques:
//pas de variables publiques

//Definitions de fonctions publiques:
int piloteSerieUSB_Balance_initialise(void)
{  
  // Ouverture du port série 
  piloteSerieUSB_Balance_fichier = open(PILOTESERIEUSB_USB0, O_RDWR | O_NOCTTY | O_NDELAY);
  if (piloteSerieUSB_Balance_fichier == -1)
  {
    piloteSerieUSB_Balance_fichier = open(PILOTESERIEUSB_USB1, O_RDWR | O_NOCTTY | O_NDELAY);
    if (piloteSerieUSB_Balance_fichier == -1)
    {
      piloteSerieUSB_Balance_fichier = open(PILOTESERIEUSB_USB2, O_RDWR | O_NOCTTY | O_NDELAY);
      if (piloteSerieUSB_Balance_fichier == -1)
      {
        piloteSerieUSB_Balance_fichier = open(PILOTESERIEUSB_USB3, O_RDWR | O_NOCTTY | O_NDELAY);
        if (piloteSerieUSB_Balance_fichier == -1)
        {
          piloteSerieUSB_Balance_fichier = open(PILOTESERIEUSB_USB4, O_RDWR | O_NOCTTY | O_NDELAY);
          if (piloteSerieUSB_Balance_fichier == -1)
          {
            piloteSerieUSB_Balance_fichier = open(PILOTESERIEUSB_USB5, O_RDWR | O_NOCTTY | O_NDELAY);
            if (piloteSerieUSB_Balance_fichier == -1)
            {
              piloteSerieUSB_Balance_fichier = open(PILOTESERIEUSB_USB6, O_RDWR | O_NOCTTY | O_NDELAY);
              if (piloteSerieUSB_Balance_fichier == -1)
              {
                perror("Erreur! Ouverture de port (Balance)");
                return EXIT_FAILURE;
              }
            }
          }
        }
      }
    }
  }
  
  struct termios SerialPortSettings; // Create the structure 
  tcgetattr(piloteSerieUSB_Balance_fichier, &SerialPortSettings); // Get the current attributes of the Serial port

  // Setting the Baud rate
  cfsetispeed(&SerialPortSettings, PILOTESERIEUSB_BALANCE_BAUDRATE_AVEC_B_AU_DEBUT); // Set Read Speed   
  cfsetospeed(&SerialPortSettings, PILOTESERIEUSB_BALANCE_BAUDRATE_AVEC_B_AU_DEBUT); // Set Write Speed  
 
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
  if (tcsetattr(piloteSerieUSB_Balance_fichier, TCSANOW, &SerialPortSettings) != 0) 
  {
    perror("Erreur! configuration des attributs du port serie");
    exit(EXIT_FAILURE);
  }

  return 0;
}

int piloteSerieUSB_Balance_termine(void)
{
  
  tcflush(piloteSerieUSB_Balance_fichier, TCIOFLUSH);   
  close(piloteSerieUSB_Balance_fichier);
  return 0;
}



int piloteSerieUSB_Balance_ecrit(char *Source, unsigned char NombreATransmettre)
{
  sleep(1);
  tcflush(piloteSerieUSB_Balance_fichier, TCOFLUSH);  
  sleep(1);//ajout pour test
  return (int)write(piloteSerieUSB_Balance_fichier,Source, NombreATransmettre);
}

int piloteSerieUSB_Balance_attendLaFinDeLEcriture(void)
{
  return (int)tcdrain(piloteSerieUSB_Balance_fichier);
}

int piloteSerieUSB_Balance_lit(char *Destination, unsigned char NombreMaximalDeLectures)
{
  int retour = read(piloteSerieUSB_Balance_fichier, Destination, NombreMaximalDeLectures);  
  tcflush(piloteSerieUSB_Balance_fichier, TCIFLUSH);  
  return retour;
}