#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

//DEFINITIONS REQUISES PAR LE PROGRAMME:

//Dependances materielles:
//pas de dependances materielles

//Dependances logicielles:
#define PILOTESERIEUSB_BRAS_BAUDRATE_AVEC_B_AU_DEBUT       B115200
#define PILOTESERIEUSB_BALANCE_BAUDRATE_AVEC_B_AU_DEBUT    B9600

//INFORMATION PUBLIQUE:
//Definitions publiques:
#define I2C_FICHIER "/dev/i2c-1" // fichier Linux representant le BUS #2
#define I2C_ADRESSE 0x29 // adresse du Device I2C MPU-9250 (motion tracking)
// Registre et ID du modèle attendu pour le capteur VL6180X
#define VL6180X_ID_REGISTRE 0x000 
#define VL6180X_ID 0xb4         

//Fonctions publiques:
int main(int argc,char** argv);

//Variables publiques:
//pas de variables publiques

#endif