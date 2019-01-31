#ifndef CLASSPLAYER_H
#define CLASSPLAYER_H
#include "./classUnit.h"
#include "./classMap.h"

#define START_MONEY 5

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


};
#endif