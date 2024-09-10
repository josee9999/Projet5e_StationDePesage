#define _GNU_SOURCE

//INCLUSIONS
#include <stdio.h>
#include <fcntl.h>   // File Control Definitions
#include <termios.h> // POSIX Terminal Control Definitions
#include <unistd.h>  // UNIX Standard Definitions
#include <errno.h>   
#include <sys/types.h> // Pour pid_t
#include <sys/wait.h> // Pour wait()
#include <stdlib.h>   // Pour exit()

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

//Definitions privees
//pas de definitions privees

//Declarations de fonctions privees:
int main_initialise(void);
void main_termine(void);

//Definitions de variables privees:
//pas de variables privees

//Definitions de fonctions privees:
int main_initialise(void)
{
  if (piloteSerieUSB_Bras_initialise() != 0)
  {
    return -1;
  }
  if(piloteI2C_initialise() != 0)
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
  if(interfaceVL6810x_initialise() != 0)
  {
    return -1;
  }
  return 0;
}

void main_termine(void)
{
  piloteSerieUSB_Bras_termine();
  piloteSerieUSB_Balance_termine();
  interfaceUArm_termine();
}
//Definitions de variables publiques:
//pas de variables publiques

//Definitions de fonctions publiques:
int main(int argc,char** argv)
{
  int pipeBras[2], pipeBalance[2];
  pid_t pidBras, pidBalance;

  pipe(pipeBras);
  pipe(pipeBalance);

  if (main_initialise())
	{
		printf("main_initialise: erreur\n");
    return 0;
  }

  //Fork pour le processus bras (enfant #1)
  if((pidBras = fork()) == 0)
  {
    close(pipeBras[1]);
    char buffer[32];
    while(1)
    {
      if(read(pipeBras[0], buffer, 32) > 0)
      {
        if(strcmp(buffer, "o") == 0)
        {
          processusBras_TrouvePoid('o');
          //processusBras_PrendrePoid('o');
        }
        else if(strcmp(buffer, "m") == 0)
        {
          processusBras_TrouvePoid('m');
          //processusBras_PrendrePoid('m');
        }
        else if(strcmp(buffer, "ApresBalance") == 0)
        {
          processusBras_DiscarterMetal();
        }
      }
    }
    close(pipeBras[0]);
    exit(0);
  }

  //Fork pour le processus balance (enfant #2)
  if((pidBalance = fork()) == 0)
  {
    float fPoidsEnfant = -1;
      close(pipeBalance[0]);
    while (1)
    {      
      fPoidsEnfant = -1;

      fPoidsEnfant = interfaceBalance_LecturePoids(piloteSerieUSB_Balance_fichier);
      fPoidsEnfant = interfaceBalance_ValiderValeur(fPoidsEnfant);

      if(fPoidsEnfant != -1)
      {
        write(pipeBalance[1], &fPoidsEnfant, sizeof(fPoidsEnfant));
      }
    }
    close(pipeBalance[1]);
    exit(0);
  }

  //Processus parent
  close(pipeBras[0]);
  close(pipeBalance[1]);
	
  while(1)
  {
    char cCouleurPoids[1];
    printf("couleur poids : ");//pour test
    scanf("%s", cCouleurPoids);//pour test
  
    if(strcmp(cCouleurPoids, "a") == 0)
    {
      break;
    }
    if(strcmp(cCouleurPoids, "o") == 0)
    {
      write(pipeBras[1], "o", strlen("o")+1);
    }
    else if(strcmp(cCouleurPoids, "m") == 0)
    {
      write(pipeBras[1], "m", strlen("m")+1);

      float fPoidsParent = -1;
      do  
      {
        read(pipeBalance[0], &fPoidsParent, sizeof(fPoidsParent));
      } while((fPoidsParent < 30) || (fPoidsParent > 100));
      
      printf("Le poids recu de la balance est : %2.2f \n",fPoidsParent);//pour test

      write(pipeBras[1],"ApresBalance", strlen("ApresBalance")+1);
    }
    else
    {
      printf("ERREUR"); //cas erreur a faire
    }
  }
  close(pipeBras[1]);
  close(pipeBalance[0]);

  kill(pidBras, SIGKILL);
  kill(pidBalance, SIGKILL);

  wait(NULL);
  wait(NULL);

	main_termine();
  return EXIT_SUCCESS;
}