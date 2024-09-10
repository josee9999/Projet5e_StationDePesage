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
#include "processusBras.h"

#include "piloteSerieUSB_Bras.c"
#include "piloteSerieUSB_Balance.c"
#include "interfaceUArm.c"
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
  if (piloteSerieUSB_Balance_initialise() != 0)
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
  piloteSerieUSB_Bras_termine();
  piloteSerieUSB_Balance_termine();
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
	
  //int retour = 0;
  //processusBras_TrouvePoid();
  //processusBras_PrendrePoid('o');
  processusBras_TrouvePoid();
  processusBras_PrendrePoid('m');
                              
	main_termine();
  return EXIT_SUCCESS;
}