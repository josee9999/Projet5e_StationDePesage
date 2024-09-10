#ifndef PILOTEI2C_H
#define PILOTEI2C_H

//Description: pilote qui permet d'utiliser un port I2C du BbB
// broches: 1 = sda, 2 = scl, 3 = 3.3V, 4 = gnd
// le beaglebone blue comprend des pull-up de 4.7k sur les lignes SCL et SDA
#include <stdint.h>
#include <linux/i2c-dev.h>

#define PILOTEI2C_INFORMATION_TRAITEE 0
#define PILOTEI2C_INFORMATION_DISPONIBLE 1
#define PILOTEI2C_MODULE_EN_ARRET 0
#define PILOTEI2C_MODULE_EN_FONCTION 1

int piloteI2C_fermeLeBus(void);
int piloteI2C_ecritDesOctets(uint8_t *Source, uint8_t NombreDOctetsAEcrire);
int piloteI2C_litDesOctets(uint8_t *Commande, uint8_t LongueurDeLaCommande, uint8_t *Destination, uint8_t NombreDOctetsALire);
int piloteI2C_configureLAdresse(uint8_t Adresse);
int piloteI2C_initialise(void);

#endif