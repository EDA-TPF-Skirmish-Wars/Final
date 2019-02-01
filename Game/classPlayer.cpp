#include "classPlayer.h"

void Player::setPlayer(teams_d color, Map map)
{
	cities = factories = units = 0;
	money = START_MONEY;
	HQCPoints = HQ_CP;
	this->map = map;
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
	money = money + (cities + 1)*START_MONEY;
}


void Player::startTurn()
{
	collectIncome();
	map.updateCP();
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

	return ans;
}


Unit* Player::buyUnit(units_d unitClass, Position pos)
{
	Unit * newUnit = nullptr;
	if (Unit::getCost(unitClass) <= money && map.posInMap(pos) && !map.IsBuildingOnTop(pos) && !map.IsUnitOnTop(pos) && !map.getFog(pos))
	{
		switch (unitClass)
		{
		case INFANTRY:
		{
			newUnit = new Unit(INFANTRY, pos, color);
		}break;
		case TANK:
		{
			newUnit = new Unit(TANK, pos, color);
		}break;
		case MEDTANK:
		{
			newUnit = new Unit(MEDTANK, pos, color);
		}break;
		case RECON:
		{
			newUnit = new Unit(RECON, pos, color);
		}break;
		case APC:
		{
			newUnit = new Unit(APC, pos, color);
		}break;
		case ANTIAIR:
		{
			newUnit = new Unit(ANTIAIR, pos, color);
		}break;
		case ARTILLERY:
		{
			newUnit = new Unit(ARTILLERY, pos, color);
		}break;
		case ROCKET:
		{
			newUnit = new Unit(ROCKET, pos, color);
		}break;
		}
		units++;
		map.addUnit(newUnit->getUnitClass(), pos, this->color);
		map.clearFog(pos);
		money = money - newUnit->getCost();
		
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

}

bool Player::loser()
{
	if ((HQCPoints == 0) || (units == 0))
		return true;
	else
		return false;
}

Map * Player::getMap() {
	return &map;
}
