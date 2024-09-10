//processusBras
//Historique: 
// 21/11/2024, Josee Girard, creation

//INCLUSIONS
#include <unistd.h>  // UNIX Standard Definitions
#include "main.h"
#include "piloteSerieUSB_Bras.h"
#include "interfaceUArm.h"
#include "processusBras.h"
#include "interfaceVL6180x.h"

//Definitions privees

//Declarations de fonctions privees:
void processusBras_Attente1s(void);

//Definitions de fonctions privees:
void processusBras_Attente1s(void)
{
  cLecture[8] = '0'; 
  cLecture[9] = '0';
  cLecture[10] = '0';
  sleep(1);
}

//Definitions de variables publiques:


//Definitions de fonctions publiques:

void processusBras_TrouvePoid(char couleur)
{
  float fDistance = 0;
  float fDistanceP = 100;
  float fDistanceG = 0;
  int iX = 175;
  int iY = -175;
  int iZ = 145;
  char cTrouveBloc = 0;
  char bufferX[3];

  interfaceUArm_BougePosition(iX, iY, iZ);
  processusBras_Attente1s();
  
  do
  {
    printf("1e move");//pour test
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
    sprintf(bufferX, "%d", iX);
  } while(cLecture[8] != bufferX[0] ||cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);

  do
  {
    printf("do\n");//pour test
    if(iY == 150)
    {
      iY =-150;
    }
    else
    {
      iY = 150;
    }
        
    interfaceUArm_BougePosition(iX, iY, iZ);
    processusBras_Attente1s();
    do
    {
      printf("2e move");//pour test
      interfaceUArm_DemandePosition(); 
      piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
      interfaceVL6180x_litUneDistance(&fDistance);

      if(fDistance<fDistanceP)
      {
        fDistanceP = fDistance;
      }

      if(fDistance>fDistanceG)
      {
        fDistanceG = fDistance;
      }

      sprintf(bufferX, "%d", iX);
      printf(" attente\n");//pour test
      printf("%c%c%c vs ",cLecture[8],cLecture[9],cLecture[10]);//pour test
      printf("%c%c%c\n",bufferX[0],bufferX[1],bufferX[2]);//pour test
    } while(cLecture[8] != bufferX[0] ||cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);

    if((fDistanceP < 1.5) && (couleur = 'm'))
    {
      cTrouveBloc = 1;
      printf("if\n");//pour test
    }
    else if((fDistanceP < 2.5) && (couleur = 'o'))
    {
      cTrouveBloc = 1;
      printf("if\n");//pour test
    }
    else 
    {
      iX += 5;
      printf("else\n");//pour test
    }

    printf("P : %2.2f\n",fDistanceP); //pour test
    printf("G : %2.2f\n",fDistanceG); //pour test

  } while(cTrouveBloc != 1);

  printf("\nbloc trouve en x\n");
  cTrouveBloc = 0;
  iY = -150;

  processusBras_Attente1s();
  interfaceUArm_BougePosition(iX, iY, iZ);
  
  do
  {
    printf("1e move Y");//pour test
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
    sprintf(bufferX, "%d", iX);
  } while(cLecture[8] != bufferX[0] ||cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);   

  iY += 5;
  do{
    interfaceUArm_BougePosition(iX, iY, iZ);
    processusBras_Attente1s();
    do
    {
      printf("2e move");//pour test
      interfaceUArm_DemandePosition(); 
      piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
      interfaceVL6180x_litUneDistance(&fDistance);
      if(fDistance == fDistanceP)
      {
        cTrouveBloc = 1;
      }
      sprintf(bufferX, "%d", iX);
      printf(" attente\n");//pour test
      printf("%c%c%c vs ",cLecture[8],cLecture[9],cLecture[10]);//pour test
      printf("%c%c%c\n",bufferX[0],bufferX[1],bufferX[2]);//pour test
    } while(cLecture[8] != bufferX[0] ||cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);

    if(cTrouveBloc = 0)
    {
      printf("if\n");
      iY += 5;
    }
  } while(cTrouveBloc != 1);
  printf("\nbloc trouve en x\n");
}

void processusBras_PrendrePoid(char couleur)
{
	interfaceUArm_demarreVentouse();
	if(couleur == 'm')
	{
		processusBras_AllerBalance();
	}
	if(couleur == 'o')
	{
		processusBras_DiscarterOrange();
	}
}

void processusBras_AllerBalance(void)
{
  processusBras_Attente1s();
	interfaceUArm_BougePosition(75,-250,150);

  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '7' || cLecture[9] != '5' );

  processusBras_Attente1s();
	interfaceUArm_BougePosition(76,-250,105);

  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '7' || cLecture[9] != '6');

  interfaceUArm_arreteVentouse();

  processusBras_Attente1s();
	interfaceUArm_BougePosition(75,-250,125);

  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '7' || cLecture[9] != '5' );
}

void processusBras_DiscarterOrange(void)
{
	processusBras_Attente1s();
	interfaceUArm_BougePosition(250,50,50);
  
  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '2' || cLecture[9] != '5' || cLecture[10] != '0');

	processusBras_Attente1s();
	interfaceUArm_BougePosition(150,50,50);

  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '1' || cLecture[9] != '5' || cLecture[10] != '0');
	
  processusBras_Attente1s();
	interfaceUArm_BougePosition(151,150,50);
  
  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '1' || cLecture[9] != '5' || cLecture[10] != '1');

	processusBras_Attente1s();
	interfaceUArm_BougePosition(130,250,23);
  
  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '1' || cLecture[9] != '3' || cLecture[10] != '0');

	interfaceUArm_arreteVentouse();

	processusBras_Attente1s();
	interfaceUArm_BougePosition(100,150,32);

  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '1' || cLecture[9] != '0' );

	processusBras_Attente1s();
	interfaceUArm_BougePosition(99,50,32);

  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '9' || cLecture[9] != '9' );

	processusBras_Attente1s();
	interfaceUArm_BougePosition(98,25,32);
  
  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '9' || cLecture[9] != '8' );
}

void processusBras_DiscarterMetal(void)
{
  processusBras_Attente1s();
	interfaceUArm_BougePosition(76,-250,105);

  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '7' || cLecture[9] != '6');

  interfaceUArm_demarreVentouse();

  processusBras_Attente1s();
	interfaceUArm_BougePosition(76,-250,150);

  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '7' || cLecture[9] != '6');

  processusBras_Attente1s();
	interfaceUArm_BougePosition(200,25,25);

  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '2' ||cLecture[9] != '0' || cLecture[10] != '0');

  processusBras_DiscarterOrange();

}