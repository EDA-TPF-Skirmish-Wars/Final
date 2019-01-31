#include "classPlayer.h"

Player::Player(teams_d color)
{
	cities = factories = units = 0;
	money = START_MONEY;
	HQCPoints = HQ_CP;
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

unsigned int Player::getState()
{
	return status;
}

void Player::collectIncome()
{
	money = money + (cities + 1)*START_MONEY;
}


void Player::updateInventory()
{
	p_inv_s temp = (*map).getPlayerInventory(color);
	this->HQCPoints = temp.HQCPoints;
	this->cities = temp.numberCities;
	this->factories = temp.numberFactories;
	this->units = temp.numberUnits;
}

Unit* Player::buyUnit(units_d unitClass, Position pos)
{
	Unit * newUnit = nullptr;
	if (status == PURCHASING && Unit::getCost(unitClass) <= money && map->posInMap(pos) && !map->IsBuildingOnTop(pos) && !map->IsUnitOnTop(pos))
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
		map->addUnit(newUnit->getUnitClass(), pos, this->color);
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


bool Player::looser()
{
	if ((HQCPoints == 0) || (units == 0))
		return true;
	else
		return false;
}
