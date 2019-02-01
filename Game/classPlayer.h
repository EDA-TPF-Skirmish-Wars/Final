#ifndef CLASSPLAYER_H
#define CLASSPLAYER_H
#include "./classUnit.h"
#include "./classMap.h"


#define START_MONEY 5


class Player {
public:
	void setPlayer(teams_d color, Map * map);
	~Player();

	unsigned int getMoney();
	unsigned int getFactories();
	unsigned int getCities();
	unsigned int getUnits();
	unsigned int getHQCPoints();
	Map * getMap();
	void endTurn();


	void collectIncome();
	void updateInventory();

	Unit * buyUnit(units_d unitClass, Position pos);
	void captureNewBuilding(buildings_d type);

	void killedUnit();
	void lostBuilding(buildings_d type);

	bool looser();
	vector<units_d> getUnitsAvailableToBuy();

private:
	teams_d color;

	unsigned int money;
	unsigned int factories;
	unsigned int cities;
	unsigned int units;

	unsigned int HQCPoints;

	Map * map;
};
#endif
