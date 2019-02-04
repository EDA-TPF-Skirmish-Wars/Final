#ifndef CLASSPLAYER_H
#define CLASSPLAYER_H
#include "./classUnit.h"
#include "./classMap.h"


#define START_MONEY 5

typedef enum playerStatus { PURCHASING, MOVE_AND_ATT, WAITING, IDLE } playerStatus_d;


class Player {
public:
	void setPlayer(teams_d color, Map map);
	~Player();

	unsigned int getMoney();
	unsigned int getFactories();
	unsigned int getCities();
	unsigned int getUnits();
	unsigned int getHQCPoints();
	Map * getMap();
	void endTurn();
	playerStatus_d getStatus();
	void setStatus(playerStatus_d status);

	void startTurn();


	void collectIncome();
	void updateInventory();

	Unit * buyUnit(units_d unitClass, Position pos, teams_d owner);
	void captureNewBuilding(buildings_d type);

	void killedUnit();
	void lostBuilding(buildings_d type);

	bool finish();
	bool enemyLose();

	bool iWin();

	vector<units_d> getUnitsAvailableToBuy();
private:
	teams_d color;

	unsigned int money;
	unsigned int factories;
	unsigned int cities;
	unsigned int units;

	playerStatus_d status;

	unsigned int HQCPoints;
	Map map;
};
#endif
