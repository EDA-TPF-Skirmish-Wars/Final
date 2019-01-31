#include "classMap.h"

Map::Map()
{
	for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
		for (unsigned int j = 0; j < BOARD_WIDTH; j++) {
			board[i][j] = nullptr;
		}
	}
}

Map::~Map()
{
	for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
		for (unsigned int j = 0; j < BOARD_WIDTH; j++) {
			delete board[i][j];
		}
	}
}

Unit Map::getUnit(Position pos)
{
	return *board[pos.row][pos.column]->unitOnTop;
}

Building Map::getBuilding(Position pos)
{
	return *board[pos.row][pos.column]->buildingOnTop;
}

Building * Map::getBuildingPtr(Position pos)
{
	return board[pos.row][pos.column]->buildingOnTop;
}

Unit * Map::getUnitPtr(Position pos)
{
	return board[pos.row][pos.column]->unitOnTop;
}

unit_type Map::getUnitType(Position pos)
{
	return board[pos.row][pos.column]->unitOnTop->getType();	
}
teams_d Map::getUnitTeam(Position pos)
{
	return board[pos.row][pos.column]->unitOnTop->getTeam();

}
teams_d Map::getBuildingTeam(Position pos)
{
	return board[pos.row][pos.column]->buildingOnTop->getBuildingTeam();
}
bool Map::getFog(Position pos)
{
	return board[pos.row][pos.column]->getFog();
}

terrains_d Map::getTerrain(Position pos)
{
	return board[pos.row][pos.column]->getTypeOfTerrain();
}

teams_d Map::getTeam()
{
	return team;
}
teams_d Map::getEnemyTeam()
{
	return enemyTeam;
}

bool Map::IsUnitOnTop(Position pos)
{
	return board[pos.row][pos.column]->unitOnTop != nullptr;
}
bool Map::IsBuildingOnTop(Position pos)
{
	return board[pos.row][pos.column]->buildingOnTop != nullptr;
}
void Map::setTeam(teams_d team)
{
	this->team = team;
}
void Map::setEnemyTeam(teams_d enemyTeam)
{
	this->enemyTeam = enemyTeam;
}

void Map::addTile(Position pos, terrains_d type, bool fog)
{
	board[pos.row][pos.column] = new Tile(pos, type, fog);
}

bool Map::addBuilding(buildings_d type, teams_d color, Position pos)
{
	if (board[pos.row][pos.column]->buildingOnTop != nullptr)
		return false;
	else
	{
		board[pos.row][pos.column]->buildingOnTop = new Building(type, color, pos);
		return true;
	}
}

bool Map::addUnit(units_d unitClass, Position pos, teams_d owner)
{
	if (board[pos.row][pos.column]->unitOnTop != nullptr)
		return false;
	else
	{
		board[pos.row][pos.column]->unitOnTop = new Unit(unitClass, pos, owner);
		return true;
	}
}

void Map::removeUnit(Position pos)
{
	board[pos.row][pos.column]->removeUnit();
}

void Map::selectTile(Position pos)
{
	board[pos.row][pos.column]->selectTile();
}

void Map::unselectTile(Position pos)
{
	board[pos.row][pos.column]->unselectTile();
}


p_inv_s Map::getPlayerInventory(teams_d color)
{
	p_inv_s temp;
	temp.HQCPoints = 0;
	temp.numberCities = 0;
	temp.numberFactories = 0;
	temp.numberUnits = 0;
	temp.team = color;

	for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
		for (unsigned int j = 0; j < BOARD_WIDTH; j++) {

			Position pos(i, j);

			if (IsBuildingOnTop(pos)) {
				if (getBuildingTeam(pos) == color)
				{
					switch (board[pos.row][pos.column]->buildingOnTop->getBuildingType())
					{
					case HQ:
					{
						temp.HQCPoints = board[pos.row][pos.column]->buildingOnTop->getCapturePoints();
					} break;
					case FACTORY:
					{
						temp.numberFactories++;
					}break;
					case CITY:
					{
						temp.numberCities++;
					}break;
					default:
						break;
					}
				}
			}

			if (IsUnitOnTop(pos)) {
				if (getUnitTeam(pos) == color) {
					temp.numberUnits++;
				}
			}
		}
	}
	return temp;
}

void Map::clearFog(Position pos)
{
	Position posR(pos.row, pos.column+1);
	Position posL(pos.row, pos.column-1);
	Position posU(pos.row+1, pos.column);
	Position posD(pos.row-1, pos.column);

	board[pos.row][pos.column]->clearFog();

	if(posInMap(posR))
		board[posR.row][posR.column]->clearFog();
	if (posInMap(posL))
		board[posL.row][posL.column]->clearFog();
	if (posInMap(posU))
		board[posU.row][posU.column]->clearFog();
	if (posInMap(posD))
		board[posD.row][posD.column]->clearFog();
}

bool Map::posInMap(Position pos)
{
	if (pos.row < BOARD_HEIGHT && pos.row >= 0 && pos.column < BOARD_WIDTH && pos.column >= 0)
		return true;
	else
		return false;
}

bool Map::buyingAvailable(Position pos)
{
	if (getBuildingPtr(pos)->getBuildingType() == FACTORY && (getBuildingPtr(pos)->getBuildingTeam() == this->team))
		return true;
	else
		return false;
}
		

bool Map::loadAvailable(Position pos)
{
	if (IsUnitOnTop(pos) && getUnitPtr(pos)->getUnitClass() == APC)
	{
		classAPC * apc = (classAPC *)getUnitPtr(pos);
		if (apc->canLoad(this->team))
			return true;
		else
			return false;
	}
	return false;
}

bool Map::unloadAvailable(Position pos, Position WhereTo)
{
	if (IsUnitOnTop(pos) && getUnitPtr(pos)->getUnitClass() == APC)
	{
		classAPC * apc = (classAPC *)getUnitPtr(pos);
		if (apc->canUnload(WhereTo))
			return true;
		else
			return false;
	}
	return false;
}

void Map::unloadAPC(Position pos, Position newPos)
{
	classAPC * apc = (classAPC *)getUnitPtr(pos);
	if (apc->canUnload(newPos))
		board[newPos.row][newPos.column]->setUnit(apc->unloadingUnitIfPossible(newPos));
}

bool Map::captureAvailable(Position pos)
{
	if (IsBuildingOnTop(pos) && (getBuildingTeam(pos) != this->team) && (!IsUnitOnTop(pos)))
		return true;
	else
		return false;
}

void Map::changeUnitPos(Unit unit, Position newPos)
{
	if (IsUnitOnTop(newPos) && (getUnitPtr(newPos)->getUnitClass() == APC))
	{
		if (unit.getType() == FOOT && loadAvailable(newPos)) {
			classAPC * apc = (classAPC *)(getUnitPtr(newPos));
			removeUnit(unit.getPosition());
			unit.ChangeUnitPosition(newPos);
			apc->loadUnitIfPossible(unit, unit.getTeam());
		}
	}
	else
	{
		if (unit.getType() == FOOT && captureAvailable(newPos))
		{
			getBuildingPtr(newPos)->captureBuilding(unit.getTeam(), unit.isReduced());
		}
		clearFog(newPos);
		removeUnit(unit.getPosition());
		unit.ChangeUnitPosition(newPos);
		board[newPos.row][newPos.column]->setUnit(&unit);
	}
}


bool Map::moveUPavailable(Position pos)
{
	Position temp = pos;
	temp.row++; //arriba
	bool valid = false;
	
	if (posInMap(temp)) //si temp esta adentro del mapa 
	{
		if (!getFog(temp)) //no puede haber fog
		{
			if (IsUnitOnTop(pos)) //si tengo una unidad para mover
			{
				if (IsUnitOnTop(temp)) //si tengo una unidad a donde me quiero mover
				{
					if (getUnitType(pos) == FOOT) //si mi unidad es un foot
					{
						if (loadAvailable(temp)) //si la unidad que hay a donde me quiero mover es un APC mio 
						{
							valid = true; //si hay una unit solo me puedo mover si hay un APC MIO
						}
					}
				}
				if (IsBuildingOnTop(temp)) //si tengo una building
				{
					if (getUnitType(pos) == FOOT) //mi unidad es foot
					{
						if (captureAvailable(temp)) //si puedo capturar la ciudad 
						{
							valid = true; //me puedo mover
						}
					}
				}
				if (!IsUnitOnTop(temp) && !IsBuildingOnTop(temp)) // si no hay nada me puedo mover
					valid = true;
			}
		}
	}
	return valid;
}

bool Map::moveDOWNavailable(Position pos)
{
	Position temp = pos;
	temp.row--; //arriba
	bool valid = false;

	if (posInMap(temp)) //si temp esta adentro del mapa 
	{
		if (!getFog(temp)) //no puede haber fog
		{
			if (IsUnitOnTop(pos)) //si tengo una unidad para mover
			{
				if (IsUnitOnTop(temp)) //si tengo una unidad a donde me quiero mover
				{
					if (getUnitType(pos) == FOOT) //si mi unidad es un foot
					{
						if (loadAvailable(temp)) //si la unidad que hay a donde me quiero mover es un APC mio 
						{
							valid = true; //si hay una unit solo me puedo mover si hay un APC MIO
						}
					}
				}
				if (IsBuildingOnTop(temp)) //si tengo una building
				{
					if (getUnitType(pos) == FOOT) //mi unidad es foot
					{
						if (captureAvailable(temp)) //si puedo capturar la ciudad 
						{
							valid = true; //me puedo mover
						}
					}
				}
				if (!IsUnitOnTop(temp) && !IsBuildingOnTop(temp)) // si no hay nada me puedo mover
					valid = true;
			}
		}
	}
	return valid;
}

bool Map::moveLEFTavailable(Position pos)
{
	Position temp = pos;
	temp.column--; //arriba
	bool valid = false;

	if (posInMap(temp)) //si temp esta adentro del mapa 
	{
		if (!getFog(temp)) //no puede haber fog
		{
			if (IsUnitOnTop(pos)) //si tengo una unidad para mover
			{
				if (IsUnitOnTop(temp)) //si tengo una unidad a donde me quiero mover
				{
					if (getUnitType(pos) == FOOT) //si mi unidad es un foot
					{
						if (loadAvailable(temp)) //si la unidad que hay a donde me quiero mover es un APC mio 
						{
							valid = true; //si hay una unit solo me puedo mover si hay un APC MIO
						}
					}
				}
				if (IsBuildingOnTop(temp)) //si tengo una building
				{
					if (getUnitType(pos) == FOOT) //mi unidad es foot
					{
						if (captureAvailable(temp)) //si puedo capturar la ciudad 
						{
							valid = true; //me puedo mover
						}
					}
				}
				if (!IsUnitOnTop(temp) && !IsBuildingOnTop(temp)) // si no hay nada me puedo mover
					valid = true;
			}
		}
	}
	return valid;
}
bool Map::moveRIGHTavailable(Position pos)
{
	Position temp = pos;
	temp.column++; //arriba
	bool valid = false;

	if (posInMap(temp)) //si pos está adentro del mapa 
	{

		if (!getFog(temp)) //no puede haber fog
		{
			if (IsUnitOnTop(pos)) //si tengo una unidad para mover
			{
				if (IsUnitOnTop(temp)) //si tengo una unidad a donde me quiero mover
				{
					if (getUnitType(pos) == FOOT) //si mi unidad es un foot
					{
						if (loadAvailable(temp)) //si la unidad que hay a donde me quiero mover es un APC mio 
						{
							valid = true; //si hay una unit solo me puedo mover si hay un APC MIO
						}
					}
				}
				if (IsBuildingOnTop(temp)) //si tengo una building
				{
					if (getUnitType(pos) == FOOT) //mi unidad es foot
					{
						if (captureAvailable(temp)) //si puedo capturar la ciudad 
						{
							valid = true; //me puedo mover
						}
					}
				}
				if (!IsUnitOnTop(temp) && !IsBuildingOnTop(temp)) // si no hay nada me puedo mover
					valid = true;
			}
		}
	}
	return valid;
}

options_s Map::getOptions(Position pos)
{
	options_s tmp;
	tmp.attackDownAvailable = false;
	tmp.attackLeftAvailable = false;
	tmp.attackRightAvailable = false;
	tmp.attackUpAvailable = false;
	tmp.buyAvailable = buyingAvailable(pos); //chequea si es factory del mismo equipo del mapa
	tmp.moveDownAvailable = moveDOWNavailable(pos);
	tmp.moveLeftAvailable = moveLEFTavailable(pos);
	tmp.moveRightAvailable = moveRIGHTavailable(pos);
	tmp.moveUpAvailable = moveUPavailable(pos);
	tmp.passAvailable = true; //???
	tmp.captureAvailable = captureAvailable(pos); //si hay building y no hay otra unit
	tmp.canLoad = loadAvailable(pos); // si es APC y puede cargar
	tmp.canUnload = false;

	Position temp = pos;

	if (IsUnitOnTop(pos))
	{
		temp.row++; //arriba
		tmp.attackUpAvailable = IsValidAttack(getUnit(pos), temp);
		tmp.canUnload = unloadAvailable(pos, temp);

		temp.row -= 2;//abajo
		tmp.attackDownAvailable = IsValidAttack(getUnit(pos), temp);
		if (tmp.canUnload == false)
			tmp.canUnload = unloadAvailable(pos, temp);

		temp.row = pos.row;
		temp.column++; //derecha
		tmp.attackRightAvailable = IsValidAttack(getUnit(pos), temp);
		if (tmp.canUnload == false)
			tmp.canUnload = unloadAvailable(pos, temp);

		temp.column -= 2; //izquierda
		tmp.attackLeftAvailable = IsValidAttack(getUnit(pos), temp);
		if (tmp.canUnload == false)
			tmp.canUnload = unloadAvailable(pos, temp);

	}
	return tmp;
}



list<Position> Map::getPossibleAttacks(Unit unit)
{
	list<Position> posibleAttacks;

	for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
		for (unsigned int j = 0; j < BOARD_WIDTH; i++) {

			Position pos(i, j);
			unsigned int dist = abs(pos.row - unit.getPosition().row) + abs(pos.column - unit.getPosition().column);
			if (dist >= unit.getMinRange() && dist <= unit.getMaxRange() && this->IsUnitOnTop(pos) && (getUnitTeam(pos) != unit.getTeam()) && (getFog(pos) == FOG_OFF))
			{
				posibleAttacks.push_back(pos);
			}
		}
	}
	return posibleAttacks;
}



bool Map::IsValidAttack(Unit unit, Position WhereTO)
{
	list<Position> attacksPossible = getPossibleAttacks(unit);
	bool valid = false;

	if (unit.getStatus() == SELECTED)
	{
		for (list<Position>::iterator it = attacksPossible.begin(); it != attacksPossible.end(); it++)
		{
			if (it->row == WhereTO.row && it->column == WhereTO.column)
				valid = true;
		}
	}

	return valid;
}

bool Map::attack(Unit unit, Position whereTo, unsigned int dice)
{
	bool valid = false;
	if (IsValidAttack(unit, whereTo) && 1 <= dice && 6 >= dice)
	{
		Unit * enemy = getUnitPtr(whereTo);
		unit_type enemyType = enemy->getType();
		int defenseRating = enemy->getDefense();

		terrains_d enemyTerrain = getTerrain(whereTo);

		buildings_d building = NO_BUILDING;
		if (IsBuildingOnTop(whereTo) == true)
		{
			building = getBuilding(whereTo).getBuildingType();
		}

		int initDamage = unit.getAttackFP(enemyType, unit.isReduced()) - defenseRating;
		int totalDamge = unit.attackDamage(initDamage, dice, enemyTerrain, building);

		enemy->setHP(enemy->getHP() - totalDamge);

		if (!enemy->isAlive()) //ver que pasa si es APC con loaded units
		{
			enemy->setStatus(DEAD);
			removeUnit(whereTo); //VER COMO SE MUESTRA EN EL OTRO MAPA
		}

		unit.setStatus(BLOCKED); //no puedo moverme despues de atacar

		valid = true;
	}
	return valid;
}


bool Map::move(Position WhereTo, Unit unit)
{
	bool valid = false;

	if (IsValidMove(unit, WhereTo))
	{
		unit.setMP(unit.getActualMP() - getMoveMPS(unit, WhereTo));

		changeUnitPos(unit, WhereTo);
		valid = true;

		if (unit.isItAPC())
		{
			((classAPC*)this)->ChangeUnitsPosition();
		}

	}

	return valid;

}

bool Map::capture(Unit unit, Position pos)
{
	bool valid = false;

	if (IsValidMove(unit, pos) && captureAvailable(pos)) {
		unit.setMP(unit.getActualMP()-getMoveMPS(unit, pos));
		changeUnitPos(unit, pos);
		unit.setStatus(BLOCKED);
		valid = true;
	}

	return valid;
}

bool Map::loadAPC(Unit unit, Position pos)
{
	bool valid = false;

	if (unit.getType() == FOOT && IsValidMove(unit, pos)) {
		unit.setMP(unit.getActualMP() - getMoveMPS(unit, pos));
		changeUnitPos(unit, pos);
		unit.setStatus( MOVING);
		valid = true;
	}

	return valid;
}


bool Map::IsValidMove(Unit unit, Position WhereTO) //VER mp!!! que devuelva los que necesita
{
	list<Position> MovesPossible = getPossibleMoves(unit.getPosition(), unit, unit.getActualMP());
	bool valid = false;

	if (unit.getStatus() == SELECTED)
	{
		for (list<Position>::iterator it = MovesPossible.begin(); it != MovesPossible.end(); it++)
		{
			if (it->row == WhereTO.row && it->column == WhereTO.column)
				valid = true;
		}
	}

	return valid;
}
/////////////////////////FALTA////////////////////////////



list<Position> Map::getPossibleMoves(Position tempPos, Unit unit, int currMPs) //incluye lugares doende se puede capturar a loadear a un apc
{
	list<Position> possibleMoves;
	for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
		for (unsigned int j = 0; j < BOARD_WIDTH; i++) {

			Position pos(i, j);

			unsigned int dist = abs(pos.row - unit.getPosition().row) + abs(pos.column - unit.getPosition().column);
			if (dist >= unit.getMinRange() && dist <= unit.getMaxRange() && this->IsUnitOnTop(pos) && (getUnitTeam(pos) != unit.getTeam()) && (getFog(pos) == FOG_OFF))
			{
				possibleMoves.push_back(pos);
			}
		}
	}
	return possibleMoves;

}


unsigned int Map::getMoveMPS(Unit unit, Position destination) {
	return 0;
}


