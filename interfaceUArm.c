//interfaceUArm
//Historique: 
//2018-11-12, Yves Roy, creation
//18/11/2024, Josée Girard, Modification pour adapter d'un interfaceMalyan vers un interfaceUArm

//INCLUSIONS
#include "main.h"
#include "piloteSerieUSB_Bras.h"
#include "interfaceUArm.h"

//Definitions privees
#define INTERFACEUARM_LONGUEUR_MAXIMALE_DES_COMMANDES 64
#define INTERFACEUARM_LONGUEUR_MAXIMALE_DES_REPONSES 64

//Declarations de fonctions privees:
int interfaceUArm_ecritUneCommande(char *cCommande, unsigned char ucLongueur);
int interfaceUArm_recoitUneReponse(char *cReponse, unsigned char ucLongueurMaximale);

//Definitions de variables privees
unsigned char interfaceUArm_commande[INTERFACEUARM_LONGUEUR_MAXIMALE_DES_COMMANDES];
unsigned char interfaceUArm_reponse[INTERFACEUARM_LONGUEUR_MAXIMALE_DES_REPONSES];

//Definitions de fonctions privees:
int interfaceUArm_ecritUneCommande(char *Commande, unsigned char Longueur)
{
  int retour;
  retour = piloteSerieUSB_ecrit(Commande, Longueur);
  if (retour != (int)Longueur)
  {
    return -1;
  }
  piloteSerieUSB_attendLaFinDeLEcriture();
  return retour;
}

int interfaceUArm_recoitUneReponse(char *cReponse, unsigned char ucLongueurMaximale)
{
	return piloteSerieUSB_lit(cReponse, ucLongueurMaximale);  
}

//Definitions de variables publiques:
//pas de variables publiques

//Definitions de fonctions publiques:
int interfaceUArm_initialise(void)
{
	return 0;
}

int interfaceUArm_termine(void)
{
  return 0;
}

int interfaceUArm_demarreVentouse(void)
{
  return interfaceUArm_ecritUneCommande("M2231 V1\n", 9);  
}

int interfaceUArm_arreteVentouse(void)
{
  return interfaceUArm_ecritUneCommande("M2231 V0\n", 9);
}

int interfaceUArm_BougePosition(int iX, int iY, int iZ)
{
  char cPosition[30];
  sprintf(cPosition, "G0 X%d Y%d Z%d F10000\n", iX, iY, iZ); 
  printf("G0 X%d Y%d Z%d F10000\n", iX, iY, iZ);
  return interfaceUArm_ecritUneCommande(cPosition, 29);
}

int interfaceUArm_BougePosition_FeedbackPossible(int iNumero, int iX, int iY, int iZ)
{
  char cPosition[32];
  sprintf(cPosition, "#%d G0 X%d Y%d Z%d F10000\n",iNumero, iX, iY, iZ); 
  printf("\n#%d G0 X%d Y%d Z%d F10000\n",iNumero, iX, iY, iZ); 
  return interfaceUArm_ecritUneCommande(cPosition, (sizeof(cPosition)-1));
}

int interfaceUArm_DemandePosition(void)
{
  return interfaceUArm_ecritUneCommande("#10 P2220\n", 10);
}

