// processusBras
// Historique:
//  21/11/2024, Josee Girard, creation

// INCLUSIONS
#include <unistd.h> // UNIX Standard Definitions
#include "main.h"
#include "piloteSerieUSB_Bras.h"
#include "interfaceUArm.h"
#include "processusBras.h"
#include "interfaceVL6180x.h"

// Definitions privees

// Declarations de fonctions privees:
void processusBras_Attente1s(void);

// Definitions de fonctions privees:
void processusBras_Attente1s(void)
{
  cLecture[8] = '0';
  cLecture[9] = '0';
  cLecture[10] = '0';
  sleep(1);
}

// Definitions de variables publiques:

// Definitions de fonctions publiques:

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
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
    sprintf(bufferX, "%d", iX);
  } while (cLecture[8] != bufferX[0] || cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);

  do
  {
    if (iY == 150)
    {
      iY = -150;
    }
    else
    {
      iY = 150;
    }

    interfaceUArm_BougePosition(iX, iY, iZ);
    processusBras_Attente1s();
    do
    {
      interfaceUArm_DemandePosition();
      piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
      interfaceVL6180x_litUneDistance(&fDistance);

      if (fDistance < fDistanceP)
      {
        fDistanceP = fDistance;
      }

      if (fDistance > fDistanceG)
      {
        fDistanceG = fDistance;
      }

      sprintf(bufferX, "%d", iX);
    } while (cLecture[8] != bufferX[0] || cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);

    if ((fDistanceP < 2.6) && (couleur = 'm'))
    {
      cTrouveBloc = 1;
    }
    else if ((fDistanceP < 2.6) && (couleur = 'o'))
    {
      cTrouveBloc = 1;
    }
    else
    {
      iX += 5;
    }
  } while (cTrouveBloc != 1);

  cTrouveBloc = 0;
  iY = -150;

  processusBras_Attente1s();
  interfaceUArm_BougePosition(iX, iY, iZ);

  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
    sprintf(bufferX, "%d", iX);
  } while (cLecture[8] != bufferX[0] || cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);

  iY += 5;
  iX += 10;
  do
  {
    interfaceUArm_BougePosition(iX, iY, iZ);
    cLecture[8] = '0';
    cLecture[9] = '0';
    cLecture[10] = '0';

    do
    {
      interfaceUArm_DemandePosition();
      piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
      interfaceVL6180x_litUneDistance(&fDistance);
      if (fDistance < 2.5)
      {
        cTrouveBloc = 1;
        return;
      }
      sprintf(bufferX, "%d", iX);
    } while (cLecture[8] != bufferX[0] || cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);

    if (cTrouveBloc == 0)
    {
      iY += 5;
    }
  } while (cTrouveBloc != 1);
}

void processusBras_PrendrePoid(void)
{
  int iX, iY, iZ;
  char bufferX[3];
  char bufferY[3];
  char bufferZ[3];
  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] > 0x39);

  bufferX[0] = cLecture[8];
  bufferX[1] = cLecture[9];
  bufferX[2] = cLecture[10];

  bufferY[0] = cLecture[16];
  bufferY[1] = cLecture[17];
  bufferY[2] = cLecture[18];

  bufferZ[0] = cLecture[24];
  bufferZ[1] = cLecture[25];
  bufferZ[2] = cLecture[26];

  iX = atoi(bufferX);
  iY = atoi(bufferY);
  iZ = atoi(bufferZ);

  iX += 55;
  iY -= 10;
  interfaceUArm_BougePosition(iX, iY, iZ);
  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
    sprintf(bufferX, "%d", iX);
  } while (cLecture[8] != bufferX[0] || cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);

  iZ -= 40;
  processusBras_Attente1s();

  interfaceUArm_BougePosition(iX, iY, iZ);
  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
    sprintf(bufferX, "%d", iX);
  } while (cLecture[8] != bufferX[0] || cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);

  interfaceUArm_demarreVentouse();

  iZ += 50;
  processusBras_Attente1s();

  interfaceUArm_BougePosition(iX, iY, iZ);
  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
    sprintf(bufferX, "%d", iX);
  } while (cLecture[8] != bufferX[0] || cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);
}

void processusBras_AllerBalance(void)
{
  processusBras_Attente1s();
  interfaceUArm_BougePosition(75, -250, 150);

  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '7' || cLecture[9] != '5');

  processusBras_Attente1s();
  interfaceUArm_BougePosition(76, -250, 105);

  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '7' || cLecture[9] != '6');

  interfaceUArm_arreteVentouse();

  processusBras_Attente1s();
  interfaceUArm_BougePosition(75, -250, 125);

  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '7' || cLecture[9] != '5');
}

void processusBras_DiscarterOrange(void)
{
  processusBras_Attente1s();
  interfaceUArm_BougePosition(250, 50, 150);
  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '2' || cLecture[9] != '5' || cLecture[10] != '0');

  processusBras_Attente1s();
  interfaceUArm_BougePosition(150, 50, 150);
  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '1' || cLecture[9] != '5' || cLecture[10] != '0');

  processusBras_Attente1s();
  interfaceUArm_BougePosition(151, 150, 150);
  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '1' || cLecture[9] != '5' || cLecture[10] != '1');

  processusBras_Attente1s();
  interfaceUArm_BougePosition(130, 250, 23);
  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '1' || cLecture[9] != '3' || cLecture[10] != '0');

  interfaceUArm_arreteVentouse();
}

void processusBras_DiscarterMetal(void)
{
  processusBras_Attente1s();
  interfaceUArm_BougePosition(76, -250, 105);

  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '7' || cLecture[9] != '6');

  interfaceUArm_demarreVentouse();

  processusBras_Attente1s();
  interfaceUArm_BougePosition(76, -250, 125);

  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '7' || cLecture[9] != '6');

  processusBras_Attente1s();
  interfaceUArm_BougePosition(120, -100, 125);

  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '1' || cLecture[9] != '2' || cLecture[10] != '0');

  processusBras_Attente1s();
  interfaceUArm_BougePosition(130, 150, 125);

  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '1' || cLecture[9] != '3' || cLecture[10] != '0');

  processusBras_Attente1s();
  interfaceUArm_BougePosition(150, 125, 32);

  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] != '1' || cLecture[9] != '5' || cLecture[10] != '0');

  interfaceUArm_arreteVentouse();
}

void processusBras_RetourBase(void)
{/*
  int iX, iY, iZ;
  char bufferX[3];
  char bufferY[3];
  char bufferZ[3];
  do
  {
    interfaceUArm_DemandePosition();
    piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
  } while (cLecture[8] > 0x39);

  bufferX[0] = cLecture[8];
  bufferX[1] = cLecture[9];
  bufferX[2] = cLecture[10];

  bufferY[0] = cLecture[16];
  bufferY[1] = cLecture[17];
  bufferY[2] = cLecture[18];

  bufferZ[0] = cLecture[24];
  bufferZ[1] = cLecture[25];
  bufferZ[2] = cLecture[26];

  do
  {
    iX = atoi(bufferX);
    iY = atoi(bufferY);
    iZ = atoi(bufferZ);

    if ((iZ > 52) || (iZ < -52))
    {
      if (iZ > 0)
      {
        iZ -= 20;
      }
      else
      {
        iZ += 20;
      }
    }
    else if ((iZ > 37) || (iZ < -37))
    {
      if (iZ > 0)
      {
        iZ -= 5;
      }
      else
      {
        iZ += 5;
      }
    }
    else
    {
      iZ = 32;

      if ((iY > 50) || (iY < -50))
      {
        if (iY > 0)
        {
          iY -= 20;
        }
        else
        {
          iY += 20;
        }
      }
      else if ((iY > 30) || (iY < -30))
      {
        if (iY > 0)
        {
          iY -= 5;
        }
        else
        {
          iY += 5;
        }
      }
      else
      {
        iY = 25;
        if ((iX >113) || (iX < -113))
        {
          if (iX > 0)
          {
            iX -= 20;
          }
          else
          {
            iX += 20;
          }
        }
        else if ((iX > 103) || (iX < -103))
        {
          if (iX > 0)
          {
            iX -= 5;
          }
          else
          {
            iX += 5;
          }
        }
        else
        {
          iX = 98;
        }
      }
    }

    processusBras_Attente1s();
    interfaceUArm_BougePosition(iX, iY, iZ);

    do
    {
      interfaceUArm_DemandePosition();
      piloteSerieUSB_Bras_lit(cLecture, (sizeof(cLecture) - 1));
      sprintf(bufferX, "%d", iX);
    } while (cLecture[8] != bufferX[0] || cLecture[9] != bufferX[1] || cLecture[10] != bufferX[2]);

  } while ((iX != 98) || (iY != 25) || (iZ != 32));*/

  interfaceUArm_arreteVentouse();
}