//processusBras
//Historique: 
// 21/11/2024, Josee Girard, creation

//INCLUSIONS
#include <unistd.h>  // UNIX Standard Definitions
#include "main.h"
#include "piloteSerieUSB_Bras.h"
#include "interfaceUArm.h"
#include "processusBras.h"


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

void processusBras_TrouvePoid(void)
{
  //A CHANGER QUAND CAPTEUR DE DISTANCE FAIT !!!!!

  interfaceUArm_BougePosition(200,25,25);
  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
    //printf(cLecture);
    //printf("\n");
  } while(cLecture[8] != '2' ||cLecture[9] != '0' || cLecture[10] != '0');

  processusBras_Attente1s();
  interfaceUArm_BougePosition(250,25,25);

  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '2' || cLecture[9] != '5' || cLecture[10] != '0');

  processusBras_Attente1s();
  interfaceUArm_BougePosition(300,0,0);

  do
  {
    interfaceUArm_DemandePosition(); 
    piloteSerieUSB_Bras_lit(cLecture,(sizeof(cLecture)-1));
  } while(cLecture[8] != '3' || cLecture[9] != '0' || cLecture[10] != '0');
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