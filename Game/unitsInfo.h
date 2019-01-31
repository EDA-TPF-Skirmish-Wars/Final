#pragma once

typedef enum { INFANTRY, MECH, RECON, TANK, MEDTANK, APC, ARTILLERY, ROCKET, ANTIAIR, ERRORCLASS }units_d;
typedef enum { FOOT, WHEEL, TREAD, ERRORTYPE} unit_type;

#define HP_MAX 8
#define HP_REDUCED 4
#define HEAL_HP 2
#define MP_MAX 99 //no se puede mover por ahí

//Infantry

#define typeInfantry   FOOT

#define fpFOOTinfantry    5 
#define fpTREADinfantry   5 
#define fpWHEELinfantry   5 

#define fpReducedFOOTinfantry    3 
#define fpReducedTREADinfantry   3 
#define fpReducedWHEELinfantry   3 

#define mcGrassInfantry   1 
#define mcRoadInfantry    1 
#define mcForestInfantry  1 
#define mcRiverInfantry   2 
#define mcHillInfantry    2 

#define rangeMaxInfantry   1 
#define rangeMinInfantry   1 

#define defenceInfantry   1 
#define mpInfantry        3 

#define costInfantry   1 

//Mech

#define typeMech   FOOT

#define fpFOOTmech    6 
#define fpTREADmech   10 
#define fpWHEELmech   10 

#define fpReducedFOOTmech    4 
#define fpReducedTREADmech   7 
#define fpReducedWHEELmech   6 

#define mcGrassMech   1 
#define mcRoadMech    1 
#define mcForestMech  1 
#define mcRiverMech   1 
#define mcHillMech    1 

#define rangeMaxMech   1 
#define rangeMinMech   1 

#define defenceMech   2 
#define mpMech   2 

#define costMech   3 


//Recon

#define typeRecon   WHEEL

#define fpFOOTrecon    7  
#define fpTREADrecon   7 
#define fpWHEELrecon   7 

#define fpReducedFOOTrecon    4 
#define fpReducedTREADrecon   6 
#define fpReducedWHEELrecon   5 

#define mcGrassRecon   2 
#define mcRoadRecon    1 
#define mcForestRecon  3 
#define mcRiverRecon   MP_MAX 
#define mcHillRecon    MP_MAX 

#define rangeMaxRecon   1 
#define rangeMinRecon   1 

#define defenceRecon   3 
#define mpRecon   8 

#define costRecon   4 


//Tank

#define typeTank   TREAD

#define fpFOOTtank    7 
#define fpTREADtank   10 
#define fpWHEELtank   10 

#define fpReducedFOOTtank    4 
#define fpReducedTREADtank   8 
#define fpReducedWHEELtank   6 

#define mcGrassTank   1 
#define mcRoadTank    1 
#define mcForestTank  2 
#define mcRiverTank   MP_MAX 
#define mcHillTank    MP_MAX 

#define rangeMaxTank   1 
#define rangeMinTank   1 

#define defenceTank   6 
#define mpTank   6 

#define costTank   7 


//MedTank

#define typeMedTank   TREAD

#define fpFOOTmedtank    10 
#define fpTREADmedtank   12 
#define fpWHEELmedtank   11 

#define fpReducedFOOTmedtank    5 
#define fpReducedTREADmedtank   10 
#define fpReducedWHEELmedtank   7 

#define mcGrassMedTank   1 
#define mcRoadMedTank    1 
#define mcForestMedTank  2 
#define mcRiverMedTank   MP_MAX 
#define mcHillMedTank    MP_MAX 

#define rangeMaxMedTank   1 
#define rangeMinMedTank   1 

#define defenceMedTank   9 
#define mpMedTank   5 

#define costMedTank   16 


//Apc

#define typeAPC   TREAD

#define fpFOOTapc    0 
#define fpTREADapc   0 
#define fpWHEELapc   0 

#define fpReducedFOOTapc    0 
#define fpReducedTREADapc   0 
#define fpReducedWHEELapc   0 

#define mcGrassApc   1 
#define mcRoadApc    1 
#define mcForestApc  2 
#define mcRiverApc   MP_MAX 
#define mcHillApc    MP_MAX 

#define rangeMaxApc   0 
#define rangeMinApc   0 

#define defenceApc   4 
#define mpApc   6 

#define costApc   5 


//Artillery

#define typeArtillery   TREAD

#define fpFOOTartillery    9 
#define fpTREADartillery   11 
#define fpWHEELartillery   9 

#define fpReducedFOOTartillery    5 
#define fpReducedTREADartillery   9 
#define fpReducedWHEELartillery   6 

#define mcGrassArtillery   1 
#define mcRoadArtillery    1 
#define mcForestArtillery  2 
#define mcRiverArtillery   MP_MAX 
#define mcHillArtillery    MP_MAX 

#define rangeMaxArtillery   3 
#define rangeMinArtillery   2 

#define defenceArtillery   4 
#define mpArtillery   5 

#define costArtillery   6 


//Rocket

#define typeRocket   WHEEL

#define fpFOOTrocket    9 
#define fpTREADrocket   11 
#define fpWHEELrocket   10 

#define fpReducedFOOTrocket    5 
#define fpReducedTREADrocket   10 
#define fpReducedWHEELrocket   7 

#define mcGrassRocket   2 
#define mcRoadRocket    1 
#define mcForestRocket  3 
#define mcRiverRocket   MP_MAX 
#define mcHillRocket    MP_MAX 

#define rangeMaxRocket   5 
#define rangeMinRocket   3 

#define defenceRocket   3 
#define mpRocket   5 

#define costRocket   15 

//AntiAir

#define typeAntiAir   TREAD

#define fpFOOTantiair   10 
#define fpTREADantiair   8 
#define fpWHEELantiair   8 

#define fpReducedFOOTantiair    6 
#define fpReducedTREADantiair   7 
#define fpReducedWHEELantiair   5 

#define mcGrassAntiAir   1 
#define mcRoadAntiAir    1 
#define mcForestAntiAir  2 
#define mcRiverAntiAir   MP_MAX 
#define mcHillAntiAir    MP_MAX 

#define rangeMaxAntiAir   1 
#define rangeMinAntiAir   1 

#define defenceAntiAir   4 
#define mpAntiAir   6 

#define costAntiAir   8




#define TOTALINITDMG 17 //-3 a 13
#define TOTALTERRAINMODIF 5 //HILL/HQ, CITY, FOREST, GRASS, ROAD/RIVER
typedef struct {
	unsigned int terrainMod;
	unsigned int dice;
}defenceMod_s;

const defenceMod_s terrainDefenceModifieres[TOTALINITDMG][TOTALTERRAINMODIF] = {
	{ { 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,1 } } , //-3
	{ { 0,0 },{ 0,0 },{ 0,1 },{ 0,1 },{ 0,2 } } ,
	{ { 0,1 },{ 0,1 },{ 0,2 },{ 0,2 },{ 0,3 } } ,
	{ { 0,1 },{ 0,2 },{ 0,3 },{ 0,4 },{ 0,5 } } ,
	{ { 0,5 },{ 1,1 },{ 1,1 },{ 1,2 },{ 1,3 } } ,
	{ { 1,3 },{ 1,5 },{ 2,1 },{ 2,2 },{ 2,3 } } ,
	{ { 2,1 },{ 2,3 },{ 2,5 },{ 3,1 },{ 3,3 } } ,
	{ { 2,4 },{ 3,1 },{ 3,4 },{ 4,1 },{ 4,3 } } , //4
	{ { 3,2 },{ 3,5 },{ 4,2 },{ 4,5 },{ 5,3 } } ,
	{ { 3,5 },{ 4,3 },{ 5,1 },{ 5,5 },{ 6,3 } } ,
	{ { 4,3 },{ 5,1 },{ 6,1 },{ 6,5 },{ 7,3 } } ,
	{ { 5,1 },{ 5,5 },{ 6,5 },{ 7,4 },{ 8,0 } } ,
	{ { 5,4 },{ 6,4 },{ 7,4 },{ 8,0 },{ 9,0 } } , //9
	{ { 6,2 },{ 7,2 },{ 8,0 },{ 10,0 },{ 10,0 } } ,
	{ { 6,5 },{ 8,0 },{11,0 },{ 11,0 },{ 11,0 } } ,
	{ { 7,3 },{ 12,0 },{ 12,0 },{ 12,0 },{ 12,1 } } ,
	{ { 8,0 },{ 13,0 },{ 13,0 },{ 13,0 },{ 13,1 } }
};
