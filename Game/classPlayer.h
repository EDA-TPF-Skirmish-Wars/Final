#ifndef CLASSPLAYER_H
#define CLASSPLAYER_H
//#include "./classUnit.h"
//#include "./classBuilding.h"
//#include "./Position.h"
//#include "./classAPC.h"
//#include "./classMap.h"
//#include "./unitsInfo.h"
//#include "./TileClass.h"

#define START_MONEY 5



typedef enum playerStatus { PURCHASING, MOVE_AND_ATT, WAITING, IDLE } playerStatus_d;

class Player {
public:
	Player(teams_d color);
	~Player();

	unsigned int getMoney();
	unsigned int getFactories();
	unsigned int getCities();
	unsigned int getUnits();
	unsigned int getHQCPoints();
	unsigned int getState();


	void collectIncome();
	void endTurn();
	void nextState();
	void updateInventory();

	Unit * buyUnit(units_d unitClass, Position pos);
	void captureNewBuilding(buildings_d type);

	void killedUnit();
	void lostBuilding(buildings_d type);

	bool looser();

private:
	teams_d color;

	unsigned int money;
	unsigned int factories;
	unsigned int cities;
	unsigned int units;

	unsigned int HQCPoints;

	Map * map;

	playerStatus_d status;
};
#endif