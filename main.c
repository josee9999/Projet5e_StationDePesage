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
#include "interfaceUArm.h"

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
  if (piloteSerieUSB_initialise() != 0)
  {
    return -1;
  }
  if (interfaceUArm_initialise() != 0)
  {
    return -1;
  }
  return 0;
}

void main_termine(void)
{
  piloteSerieUSB_termine();
  interfaceUArm_termine();
}
//Definitions de variables publiques:
//pas de variables publiques

//Definitions de fonctions publiques:
int main(int argc,char** argv)
{
	if (main_initialise())
	{
		printf("main_initialise: erreur\n");
    return 0;
  }
	char cLecture[25];
  int retour = 0;

	retour = interfaceUArm_BougePosition(200,25,25);
  while(cLecture[8] != '2' && cLecture[9] != '0' && cLecture[10] != '0')
  {
    retour = interfaceUArm_DemandePosition(); 
    piloteSerieUSB_lit(cLecture,(sizeof(cLecture)-1));
    printf(cLecture);
    printf("\n");
  }

  cLecture[8] = '0'; 
  cLecture[9] = '0';
  cLecture[10] = '0';
  interfaceUArm_BougePosition(250,25,25);
  sleep(1);

  interfaceUArm_DemandePosition(); 
  piloteSerieUSB_lit(cLecture,(sizeof(cLecture)-1));
  while(cLecture[8] != '2' && cLecture[9] != '5' && cLecture[10] != '0')
  {
    retour = interfaceUArm_DemandePosition(); 
    piloteSerieUSB_lit(cLecture,(sizeof(cLecture)-1));
    printf(cLecture);
    printf("\n");
  }

  cLecture[8] = '0'; 
  cLecture[9] = '0';
  cLecture[10] = '0';
  interfaceUArm_BougePosition(300,0,0);
  sleep(1);

  interfaceUArm_DemandePosition(); 
  piloteSerieUSB_lit(cLecture,(sizeof(cLecture)-1));
  while(cLecture[8] != '3' && cLecture[9] != '0' && cLecture[10] != '0')
  {
    retour = interfaceUArm_DemandePosition(); 
    piloteSerieUSB_lit(cLecture,(sizeof(cLecture)-1));
    printf(cLecture);
    printf("\n");
    
  }
  cLecture[8] = '0'; 
  cLecture[9] = '0';
  cLecture[10] = '0';
  /*
  while(cLecture[0] != '$')
  {
    cLecture[0] = 0;
    piloteSerieUSB_lit(cLecture,(sizeof(cLecture)-1));
    printf(cLecture);
  }*/
  
 // piloteSerieUSB_lit(cLecture,(sizeof(cLecture)-1));

  //printf("\n1e lecture\n");
  //printf(cLecture);
  //printf("\nwhile\n");

  //while((/*cLecture[0] != '$' && cLecture[1] != '1' && */cLecture[2] != '5'))
  //{
  //  for(int i = 0; i++; i=25)
  //  {
  //    cLecture[i] = '0';
  //  }
  //  piloteSerieUSB_lit(cLecture, (sizeof(cLecture)-1));  
  //}

  //printf("\n2e lecture\n");
  //printf(cLecture);
  
  //sleep(3);
  
  
  //for(int i = 0; i++; i=25)
  //{
  //  cLecture[i] = '0';
  //}
  //printf("\n2e while\n");

  /*while(/*cLecture[0] != '$' && cLecture[1] != '2' && *//*cLecture[2] != '8' )
  {
    for(int i = 0; i++; i=25)
    {
      cLecture[i] = '0';
    }*/
  //  piloteSerieUSB_lit(cLecture, (sizeof(cLecture)-1));  
 //   printf("3e lecture\n");
 //   printf(cLecture);
  //}*/
 // piloteSerieUSB_lit(cLecture, (sizeof(cLecture)-1));  
 // printf("\n4e lecture\n");
//  printf(cLecture);

	main_termine();
  return EXIT_SUCCESS;
}