#include "classPlayer.h"

Player::Player(teams_d color)
{
	cities = factories = units = 0;
	money = START_MONEY;
	HQCPoints = HQ_CP;
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
	if (status == PURCHASING && Unit::getCost(unitClass) <= money)
	{
		newUnit = Unit::boughtUnit(unitClass, pos, color);
		units++;
		money = money - Unit::getCost(unitClass);
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

void Player::endTurn()
{
	updateInventory();
	status = WAITING;
}
void Player::nextState()
{
	switch (status) 
	{
	case WAITING:
	{
		status = MOVE_AND_ATT;
	}break;
	case MOVE_AND_ATT:
	{
		status = PURCHASING;
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
