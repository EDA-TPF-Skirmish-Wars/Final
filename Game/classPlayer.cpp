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
	updateInventory();
	map.endTurnUnits();
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


Unit* Player::buyUnit(units_d unitClass, Position pos, teams_d owner)
{
	Unit * newUnit = nullptr;
	if (Unit::getCost(unitClass) <= money)
	{
		Position temp = pos;
		temp.row--;

		if (!(map.posInMap(temp) && !map.IsBuildingOnTop(temp) && !map.IsUnitOnTop(temp) && !map.getFog(temp)))
		{
			temp.row += 2;
			if (!(map.posInMap(temp) && !map.IsBuildingOnTop(temp) && !map.IsUnitOnTop(temp) && !map.getFog(temp)))
			{
				temp.row = pos.row;
				temp.column++;
				if (!(map.posInMap(temp) && !map.IsBuildingOnTop(temp) && !map.IsUnitOnTop(temp) && !map.getFog(temp)))
				{
					temp.column -= 2;
				}
			}
		}

		switch (unitClass)
		{
		case INFANTRY:
		{
			newUnit = new Unit(INFANTRY, temp, owner);
		}break;
		case TANK:
		{
			newUnit = new Unit(TANK, temp, owner);
		}break;
		case MEDTANK:
		{
			newUnit = new Unit(MEDTANK, temp, owner);
		}break;
		case RECON:
		{
			newUnit = new Unit(RECON, temp, owner);
		}break;
		case APC:
		{
			newUnit = new Unit(APC, temp, owner);
		}break;
		case ANTIAIR:
		{
			newUnit = new Unit(ANTIAIR, temp, owner);
		}break;
		case ARTILLERY:
		{
			newUnit = new Unit(ARTILLERY, temp, owner);
		}break;
		case ROCKET:
		{
			newUnit = new Unit(ROCKET, temp, owner);
		}break;
		}
		units++;
		map.getTilePtr(temp)->setUnit(newUnit);
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
	map.endTurnUnits();

}

bool Player::loser()
{
	if ((HQCPoints == 0) || (units == 0))
		return true;
	else
		return false;
}

Map * Player::getMap() {
	return &this->map;
}
