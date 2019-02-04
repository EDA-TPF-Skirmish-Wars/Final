#include "classPlayer.h"

void Player::setPlayer(teams_d color, Map map)
{
	cities = factories = units = 0;
	money = START_MONEY;
	HQCPoints = HQ_CP;
	this->map = map;
	this->color = color;
	updateInventory();
	status = IDLE;

}

Player::~Player()
{}

unsigned int Player :: getMoney()
{
	return money;
}
unsigned int Player::getFactories()
{
	return factories;
}

unsigned int Player::getCities()
{
	return cities;
}

unsigned int Player::getUnits()
{
	return units;
}
unsigned int Player::getHQCPoints()
{
	return HQCPoints;
}

playerStatus_d Player::getStatus()
{
	return status;
}


void Player::collectIncome()
{
	money = money + (cities + 1)*START_MONEY; //cuentan las cities y el HQ (1)
}



void Player::startTurn()
{
	collectIncome();
	map.endTurnUnits();
	map.updateCP();
	updateInventory();
	status = MOVE_AND_ATT;
	
}

void Player::updateInventory()
{
	p_inv_s temp = map.getPlayerInventory(color);
	this->HQCPoints = temp.HQCPoints;
	this->cities = temp.numberCities;
	this->factories = temp.numberFactories;
	this->units = temp.numberUnits;
}

vector<units_d> Player::getUnitsAvailableToBuy()
{
	vector<units_d> ans;

	if (Unit::getCost(INFANTRY) <= money)
		ans.push_back(INFANTRY);
	if (Unit::getCost(TANK) <= money)
		ans.push_back(TANK);
	if (Unit::getCost(MEDTANK) <= money)
		ans.push_back(MEDTANK);
	if (Unit::getCost(RECON) <= money)
		ans.push_back(RECON);
	if (Unit::getCost(APC) <= money)
		ans.push_back(APC);
	if (Unit::getCost(ANTIAIR) <= money)
		ans.push_back(ANTIAIR);
	if (Unit::getCost(ARTILLERY) <= money)
		ans.push_back(ARTILLERY);
	if (Unit::getCost(ROCKET) <= money)
		ans.push_back(ROCKET);
	if (Unit::getCost(MECH) <= money)
		ans.push_back(MECH);

	return ans;
}


Unit* Player::buyUnit(units_d unitClass, Position pos, teams_d owner)
{
	Unit * newUnit = nullptr;
	if (unitClass != NO_CLASS) {
		if (Unit::getCost(unitClass) <= money)
		{
			switch (unitClass)
			{
			case INFANTRY:
			{
				newUnit = new Unit(INFANTRY, pos, owner);
			}break;
			case TANK:
			{
				newUnit = new Unit(TANK, pos, owner);
			}break;
			case MEDTANK:
			{
				newUnit = new Unit(MEDTANK, pos, owner);
			}break;
			case RECON:
			{
				newUnit = new Unit(RECON, pos, owner);
			}break;
			case APC:
			{
				newUnit = new Unit(APC, pos, owner);
			}break;
			case ANTIAIR:
			{
				newUnit = new Unit(ANTIAIR, pos, owner);
			}break;
			case ARTILLERY:
			{
				newUnit = new Unit(ARTILLERY, pos, owner);
			}break;
			case ROCKET:
			{
				newUnit = new Unit(ROCKET, pos, owner);
			}break;
			case MECH:
			{
				newUnit = new Unit(MECH, pos, owner);
			}
			}
			units++;
			map.getTilePtr(pos)->setUnit(newUnit);
			map.clearFog(pos);
			money = money - newUnit->getCost();


		}
	}

	return newUnit;
}


void Player::captureNewBuilding(buildings_d type)
{
	switch (type) 
	{
		case CITY:
		{
			cities++;
		}break;
		case FACTORY:
		{
			factories++;
		}break;
		default:
			break;
	}
}

void Player::setStatus(playerStatus_d status)
{
	this->status = status;
}

void Player::killedUnit()
{
	if (units)//si no son cero
		units--;	
}

void Player::lostBuilding(buildings_d type)
{
	switch (type)
	{
	case CITY:
	{
		if (cities)
			cities--;
	}break;
	case FACTORY:
	{
		if (factories)
			factories--;
	}break;
	default:
		break;
	}
}

void Player::endTurn()
{
	status = WAITING;
	updateInventory();
	map.endTurnUnits();

}

bool Player::finish()
{
	updateInventory();
	if ((HQCPoints == 0) || (units == 0) || enemyLose())
		return true;

	else
		return false;
}

bool Player::enemyLose()
{
	p_inv_s temp = map.getPlayerInventory(map.getEnemyTeam());
	
	if (temp.numberUnits == 0 || temp.HQCPoints == 0)
		return true;
	else
		return false;
}



Map * Player::getMap() {
	return &this->map;
}
