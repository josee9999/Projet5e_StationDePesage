#ifndef PROCESSUSBRAS_H
#define PROCESSUSBRAS_H

//HISTORIQUE:
// 21/11/2024, Josee Girard, creation

//DEFINITIONS REQUISES PAR LE MODULE:
//Dependances materielles
//(copiez et adaptez ce qui suit dans "main.h")
//pas de dependances materielles

//Dependances logicielles
//(copiez et adaptez ce qui suit dans "main.h")
//pas de dependances logicielles

//INFORMATION PUBLIQUE:
//Definitions publiques:
//pas de definitions publiques

//Fonctions publiques:
void processusBras_TrouvePoid(char couleur);
void processusBras_PrendrePoid(char couleur);
void processusBras_AllerBalance(void);
void processusBras_DiscarterOrange(void);
void processusBras_DiscarterMetal(void);

//Variables publiques:
char cLecture[32];

#endif