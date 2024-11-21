#ifndef PILOTESERIEUSB_BRAS_H
#define PILOTESERIEUSB_BRAS_H

//MODULE: piloteSerieUSB
//DESCRIPTION: Module qui permet d'envoyer des commandes a une imprimante 3D
//de marque Malyan (M200) et de recevoir les reponses a ces commandes.

//HISTORIQUE:
// 2018-10-11, Yves Roy, creation

//DEFINITIONS REQUISES PAR LE MODULE:
//Dependances materielles
//(copiez et adaptez ce qui suit dans "main.h")
//pas de dependances materielles

//Dependances logicielles
//(copiez et adaptez ce qui suit dans "main.h")
//#define PILOTESERIEUSB_BRAS_BAUDRATE_AVEC_B_AU_DEBUT   B115200


//INFORMATION PUBLIQUE:
//Definitions publiques:
//pas de definitions publiques

//Fonctions publiques:
int piloteSerieUSB_Bras_initialise(void);
int piloteSerieUSB_Bras_termine(void);
int piloteSerieUSB_Bras_ecrit(char *Source, unsigned char NombreATransmettre);
int piloteSerieUSB_Bras_attendLaFinDeLEcriture(void);
int piloteSerieUSB_Bras_lit(char *Destination, unsigned char NombreMaximalDeLectures);

//Variables publiques:
//pas de variables publiques

#endif