#define _GNU_SOURCE
#include "main.h"
#include <linux/i2c-dev.h> //for IOCTL defs
#include <stdio.h>
#include <fcntl.h>   // File Control Definitions
#include <termios.h> // POSIX Terminal Control Definitions
#include <unistd.h>  // UNIX Standard Definitions
#include <errno.h>   
#include <sys/types.h> // Pour pid_t
#include <sys/wait.h> // Pour wait()
#include <stdlib.h>   // Pour exit()
#include <sys/stat.h>
#include <sys/ioctl.h>     
#include "piloteI2C.h"
#define PILOTEI2C_FICHIER "/dev/i2c-1" 

//variables privées
uint8_t piloteI2C_adresse;
int piloteI2C_fichier;

//fonctions privées
//pas de fonctions privées

//variables publiques
//pas de variables publiques

//fonctions publiques
int piloteI2C_fermeLeBus(void)
{
	if (close(piloteI2C_fichier) < 0)
	{
		printf("erreur: piloteI2C_fermeLeBus\n");
	    return -1;
	}
    piloteI2C_adresse = 0;
	return 0;
}

int piloteI2C_ecritDesOctets(uint8_t *Source, uint8_t NombreDOctetsAEcrire)
{
	if(write(piloteI2C_fichier, Source, NombreDOctetsAEcrire) != NombreDOctetsAEcrire)
	{
		printf("erreur: piloteI2C_ecritDesOctets\n");
		close(piloteI2C_fichier);
		return -1;
	}
	return 0;
}

int piloteI2C_litDesOctets(uint8_t *Commande, uint8_t LongueurDeLaCommande, uint8_t *Destination, uint8_t NombreDOctetsALire)
{
	if (piloteI2C_ecritDesOctets(Commande, LongueurDeLaCommande) < 0)
  {
  	printf("erreur: piloteI2C_litDesOctets 1\n");
    close(piloteI2C_fichier);
    return -1;
  }
	if (read(piloteI2C_fichier, Destination, NombreDOctetsALire) != NombreDOctetsALire)
	{
		printf("erreur: piloteI2C_litDesOctets 2\n");
		close(piloteI2C_fichier);
		return -1;
	}
	return 0;
}

int piloteI2C_configureLAdresse(uint8_t Adresse)
{
	if(piloteI2C_adresse == Adresse)
	{
		return 0;
	}
	if(ioctl(piloteI2C_fichier, I2C_SLAVE, Adresse) < 0)
	{
		printf("erreur: piloteI2C_configureLAdresse\n");
		close(piloteI2C_fichier);
		return -1;
	}
	piloteI2C_adresse = Adresse;
	return 0;
}

int piloteI2C_initialise(void)
{
	piloteI2C_adresse = 0x00;
	piloteI2C_fichier = open(PILOTEI2C_FICHIER, O_RDWR);
	if(piloteI2C_fichier == -1)
	{
		printf("erreur: pilote_initialilse 1\n");
		return -1;
	}
	if(ioctl(piloteI2C_fichier, I2C_SLAVE, piloteI2C_adresse) < 0)
	{
		printf("erreur: piloteI2C_initialise 2\n");
		close(piloteI2C_fichier);
		return -1;
	}
	return 0;
}
