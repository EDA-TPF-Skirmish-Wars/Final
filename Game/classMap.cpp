#include "classMap.h"

Map::Map()
{
	for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
		for (unsigned int j = 0; j < BOARD_WIDTH; j++) {
			board[i][j] = nullptr;
		}
	}
}

//Map::~Map()
//{
//	for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
//		for (unsigned int j = 0; j < BOARD_WIDTH; j++) {
//			delete board[i][j];
//		}
//	}
//}


Tile * Map::getTilePtr(Position pos)
{
	return board[pos.row][pos.column];
}

Unit Map::getUnit(Position pos)
{
	return *board[pos.row][pos.column]->getUnit();
}

Building Map::getBuilding(Position pos)
{
	return *board[pos.row][pos.column]->getBuilding();
}

Building * Map::getBuildingPtr(Position pos)
{
	return board[pos.row][pos.column]->getBuilding();
}

Unit * Map::getUnitPtr(Position pos)
{
	return board[pos.row][pos.column]->getUnit();
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
	return board[pos.row][pos.column]->getUnit() != nullptr;
}
bool Map::IsBuildingOnTop(Position pos)
{
	return board[pos.row][pos.column]->getBuilding() != nullptr;
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
	return;
}

bool Map::addBuilding(buildings_d type, teams_d color, Position pos)
{
	if (board[pos.row][pos.column]->getBuilding() != nullptr)
		return false;
	else
	{
		board[pos.row][pos.column]->setBuilding(new Building(type, color, pos));
		return true;
	}
}

bool Map::addUnit(units_d unitClass, Position pos, teams_d owner)
{
	if (board[pos.row][pos.column]->getUnit() != nullptr)
		return false;
	else
	{
		board[pos.row][pos.column]->setUnit( new Unit(unitClass, pos, owner));
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
					switch (board[pos.row][pos.column]->getBuilding()->getBuildingType())
					{
					case HQ:
					{
						temp.HQCPoints = board[pos.row][pos.column]->getBuilding()->getCapturePoints();
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
	if (IsBuildingOnTop(pos) && getBuildingPtr(pos)->getBuildingType() == FACTORY && (getBuildingPtr(pos)->getBuildingTeam() == this->team))
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

void Map::changeUnitPos(Unit * unit, Position newPos)
{
	if (IsUnitOnTop(newPos) && (getUnitPtr(newPos)->getUnitClass() == APC))
	{
		if (unit->getType() == FOOT && loadAvailable(newPos)) {
			classAPC * apc = (classAPC *)(getUnitPtr(newPos));
			removeUnit(unit->getPosition());
			unit->ChangeUnitPosition(newPos);
			apc->loadUnitIfPossible(*unit, unit->getTeam());
		}
	}
	else
	{
		if (unit->getType() == FOOT && captureAvailable(newPos))
		{
			getBuildingPtr(newPos)->captureBuilding(unit->getTeam(), unit->isReduced());
		}
		clearFog(newPos);
		removeUnit(unit->getPosition());
		unit->ChangeUnitPosition(newPos);
		board[newPos.row][newPos.column]->setUnit(unit);
	}
}


bool Map::moveUPavailable(Position pos)
{
	Position temp = pos;
	temp.row--; //arriba
	bool valid = false;
	
	if (posInMap(temp)) //si temp esta adentro del mapa 
	{
		if (!getFog(temp)) //no puede haber fog
		{
			if (IsUnitOnTop(pos) )  //si tengo una unidad para mover
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
	temp.row++; //abajo
	bool valid = false;

	if (posInMap(temp)) //si temp esta adentro del mapa 
	{
		if (!getFog(temp)) //no puede haber fog
		{
			if (IsUnitOnTop(pos) ) //si tengo una unidad para mover
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
	temp.column--; //izquierda
	bool valid = false;

	if (posInMap(temp)) //si temp esta adentro del mapa 
	{
		if (!getFog(temp)) //no puede haber fog
		{
			if (IsUnitOnTop(pos) ) //si tengo una unidad para mover
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
	temp.column++; //derecha
	bool valid = false;

	if (posInMap(temp)) //si pos está adentro del mapa 
	{

		if (!getFog(temp)) //no puede haber fog
		{
			if (IsUnitOnTop(pos) ) //si tengo una unidad para mover
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
	tmp.moveDownAvailable = false;
	tmp.moveLeftAvailable = false;
	tmp.moveRightAvailable = false;
	tmp.moveUpAvailable = false;
	tmp.passAvailable = true;
	tmp.captureAvailable = captureAvailable(pos); //si hay building y no hay otra unit
	tmp.canLoad = (loadAvailable(pos) && IsUnitOnTop(pos) && getUnitTeam(pos) == this->team); // si es APC y puede cargar
	tmp.canUnload = false;
	tmp.HP = -1;
	tmp.MP = -1;

	Position temp = pos;

	if (IsUnitOnTop(pos))
	{
		temp.row--; //arriba
		tmp.attackUpAvailable = (IsValidAttack(getUnitPtr(pos), temp) && IsUnitOnTop(pos) && getUnitTeam(pos) == this->team);
		tmp.canUnload = (unloadAvailable(pos, temp) && IsUnitOnTop(pos) && getUnitTeam(pos) == this->team);
		tmp.moveUpAvailable = IsValidMove(getUnitPtr(pos), temp);


		temp.row += 2;//abajo
		tmp.attackDownAvailable = (IsValidAttack(getUnitPtr(pos), temp) && IsUnitOnTop(pos) && getUnitTeam(pos) == this->team);
		tmp.moveDownAvailable = IsValidMove(getUnitPtr(pos), temp);
		if (tmp.canUnload == false)
			tmp.canUnload = (unloadAvailable(pos, temp) && IsUnitOnTop(pos) && getUnitTeam(pos) == this->team);

		temp.row = pos.row;
		temp.column++; //derecha
		tmp.attackRightAvailable = (IsValidAttack(getUnitPtr(pos), temp) && IsUnitOnTop(pos) && getUnitTeam(pos) == this->team);
		tmp.moveRightAvailable = IsValidMove(getUnitPtr(pos), temp);
		if (tmp.canUnload == false)
			tmp.canUnload = (unloadAvailable(pos, temp) && IsUnitOnTop(pos) && getUnitTeam(pos) == this->team);

		temp.column -= 2; //izquierda
		tmp.attackLeftAvailable = (IsValidAttack(getUnitPtr(pos), temp) && IsUnitOnTop(pos) && getUnitTeam(pos) == this->team);
		tmp.moveLeftAvailable = IsValidMove(getUnitPtr(pos), temp);
		if (tmp.canUnload == false)
			tmp.canUnload = (unloadAvailable(pos, temp) && IsUnitOnTop(pos) && getUnitTeam(pos) == this->team);

		tmp.HP = getUnitPtr(pos)->getHP();
		tmp.MP = getUnitPtr(pos)->getActualMP();
	}
	return tmp;
}



list<Position> Map::getPossibleAttacks(Unit * unit)
{
	list<Position> posibleAttacks;

	for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
		for (unsigned int j = 0; j < BOARD_WIDTH; j++) {

			Position pos(i, j);
			unsigned int dist = abs(pos.row - unit->getPosition().row) + abs(pos.column - unit->getPosition().column);
			if (dist >= unit->getMinRange() && dist <= unit->getMaxRange() && IsUnitOnTop(pos) && (getUnitTeam(pos) != unit->getTeam()) && (getFog(pos) == FOG_OFF))
			{
				posibleAttacks.push_back(pos);
			}
		}
	}
	return posibleAttacks;
}

list<Position> Map::getPossibleEnemyAttacks(Unit * unit)
{
	list<Position> posibleAttacks;

	for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
		for (unsigned int j = 0; j < BOARD_WIDTH; j++) {

			Position pos(i, j);
			unsigned int dist = abs(pos.row - unit->getPosition().row) + abs(pos.column - unit->getPosition().column);
			if (dist >= unit->getMinRange() && dist <= unit->getMaxRange() && this->IsUnitOnTop(pos) && (getUnitTeam(pos) != unit->getTeam()))
			{
				posibleAttacks.push_back(pos);
			}
		}
	}
	return posibleAttacks;
}

bool Map::IsValidEnemyAttack(Unit * unit, Position WhereTO)
{
	list<Position> attacksPossible = getPossibleEnemyAttacks(unit);
	bool valid = false;

		for (list<Position>::iterator it = attacksPossible.begin(); it != attacksPossible.end(); it++)
		{
			if (it->row == WhereTO.row && it->column == WhereTO.column)
				valid = true;
		}
	

	return valid;
}


bool Map::enemyAttack(Unit * unit, Position whereTo, unsigned int dice)
{

	bool valid = true;
	if (unit != nullptr) {

		if (/*IsValidEnemyAttack(unit, whereTo) &&*/ 1 <= dice && 6 >= dice && unit->getTeam() != this->team)
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

			int initDamage = unit->getAttackFP(enemyType, unit->isReduced()) - defenseRating;
			int totalDamge = unit->attackDamage(initDamage, dice, enemyTerrain, building);

			enemy->setHP(enemy->getHP() - totalDamge);

			if (!enemy->isAlive()) //ver que pasa si es APC con loaded units
			{
				enemy->setStatus(DEAD);
				removeUnit(whereTo); //VER COMO SE MUESTRA EN EL OTRO MAPA
			}

			if (!unit->isAlive()) //ver que pasa si es APC con loaded units
			{
				unit->setStatus(DEAD);
				removeUnit(unit->getPosition()); //VER COMO SE MUESTRA EN EL OTRO MAPA
			}
			valid = true;
		}
		else if (dice == 0) {
			valid = true;
		}
	}
	return valid;
}


bool Map::IsValidAttack(Unit * unit, Position WhereTO)
{
	list<Position> attacksPossible = getPossibleAttacks(unit);
	bool valid = false;

	if (unit->getStatus() != BLOCKED && unit->getStatus() != DEAD)
	{
		for (list<Position>::iterator it = attacksPossible.begin(); it != attacksPossible.end(); it++)
		{
			if (it->row == WhereTO.row && it->column == WhereTO.column)
				valid = true;
		}
	}

	return valid;
}

bool Map::attack(Unit * unit, Position whereTo, unsigned int dice)
{
	bool valid = true;

	if (unit != nullptr) {

		if (IsValidAttack(unit, whereTo) && 1 <= dice && 6 >= dice && unit->getTeam() == this->team)
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

			int initDamage = unit->getAttackFP(enemyType, unit->isReduced()) - defenseRating;
			int totalDamge = unit->attackDamage(initDamage, dice, enemyTerrain, building);

			enemy->setHP(enemy->getHP() - totalDamge);

			if (!enemy->isAlive()) //ver que pasa si es APC con loaded units
			{
				enemy->setStatus(DEAD);
				removeUnit(whereTo); //VER COMO SE MUESTRA EN EL OTRO MAPA
			}

			unit->setStatus(BLOCKED); //no puedo moverme despues de atacar

			if (!unit->isAlive()) //ver que pasa si es APC con loaded units
			{
				unit->setStatus(DEAD);
				removeUnit(unit->getPosition()); //VER COMO SE MUESTRA EN EL OTRO MAPA
			}



			valid = true;
		}
		else if (dice == 0) {
			valid = true;
		}
	}
	return valid;
}


bool Map::move(Position WhereTo, Unit * unit)
{
	bool valid = false;

	if (IsValidMove(unit, WhereTo))
	{
		if (IsBuildingOnTop(unit->getPosition()))
			getBuilding(unit->getPosition()).resetCapturePoints();

		unit->setMP(unit->getActualMP() - getMoveMPS(unit, WhereTo));

		changeUnitPos(unit, WhereTo);
		valid = true;

		if (unit->isItAPC())
		{
			((classAPC*)this)->ChangeUnitsPosition();
		}
	}

	return valid;

}

bool Map::capture(Unit * unit, Position pos)
{
	bool valid = false;

	if (IsValidMove(unit, pos) && captureAvailable(pos)) {
		unit->setMP(unit->getActualMP()-getMoveMPS(unit, pos));
		changeUnitPos(unit, pos);
		unit->setStatus(BLOCKED);
		valid = true;
	}

	return valid;
}

bool Map::loadAPC(Unit * unit, Position pos)
{
	bool valid = false;

	if (unit->getType() == FOOT && IsValidMove(unit, pos)) {
		unit->setMP(unit->getActualMP() - getMoveMPS(unit, pos));
		changeUnitPos(unit, pos);
		unit->setStatus( MOVING);
		valid = true;
	}

	return valid;
}


bool Map::IsValidMove(Unit * unit, Position WhereTO) //VER mp!!! que devuelva los que necesita
{
	list<moves_s> MovesPossible;
	MovesPossible.clear();
	moves_s temp;
	temp.movingPoints = 0;
	temp.destination = unit->getPosition();
	bool valid = false;

	if (unit->getTeam() == this->team)
	{

		getPossibleMoves(unit, unit->getActualMP(), temp, &MovesPossible);

		if (unit->getStatus() != BLOCKED && unit->getStatus() != DEAD)
		{
			for (list<moves_s>::iterator it = MovesPossible.begin(); it != MovesPossible.end(); it++)
			{
				if (it->destination == WhereTO)
					valid = true;
			}
		}
	}

	return valid;
}


bool Map::IsValidEnemyMove(Unit * unit, Position WhereTO) //VER mp!!! que devuelva los que necesita
{
	list<moves_s> MovesPossible;
	MovesPossible.clear();
	moves_s temp;
	temp.movingPoints = 0;
	temp.destination = unit->getPosition();
	bool valid = false;

	if (unit->getTeam() != this->team)
	{

		getPossibleMoves(unit, unit->getActualMP(), temp, &MovesPossible);

		if (unit->getStatus() != BLOCKED && unit->getStatus() != DEAD)
		{
			for (list<moves_s>::iterator it = MovesPossible.begin(); it != MovesPossible.end(); it++)
			{
				if (it->destination == WhereTO)
					valid = true;
			}
		}
	}

	return valid;
}


void Map::getPossibleMoves(Unit * unit, int currMPs, moves_s temp2, list<moves_s> * moves) //incluye lugares doende se puede capturar a loadear a un apc
{
	moves_s temp = temp2;

	//arriba
	if (moveUPavailable(temp.destination)) //chequea fog y que no haya units ni buildings a donde voy a no ser que puedan ser capturadas o sea un apc que puedo loadear
	{
		temp.destination.row--; //estoy arriba 
		terrains_d nextTerrain = getTerrain(temp.destination);
		temp.movingPoints += unit->getTerrainMC(nextTerrain);
		int tempMps = currMPs - temp.movingPoints;
		if (tempMps >= 0)
		{
			bool differentTile = true;
			for (list<moves_s>::iterator it = moves->begin(); it != moves->end(); it++)
			{
				if (it->destination == temp.destination)
				{
					differentTile = false; // ya existe el camino en la lista
					if (it->movingPoints > temp.movingPoints)
						it->movingPoints = temp.movingPoints;

				}
			}

			if (differentTile == true)
				moves->push_back(temp);

			getPossibleMoves(unit, tempMps, temp, moves);

		}
	}
	temp = temp2;

	//abajo
	if (moveDOWNavailable(temp.destination))
	{
		temp.destination.row++; //estoy abajo 
		terrains_d nextTerrain = getTerrain(temp.destination);
		temp.movingPoints += unit->getTerrainMC(nextTerrain);
		int tempMps = currMPs - temp.movingPoints;
		if (tempMps >= 0)
		{
			bool differentTile = true;
			for (list<moves_s>::iterator it = moves->begin(); it != moves->end(); it++)
			{
				if (it->destination == temp.destination)
				{
					differentTile = false; // ya existe el camino en la lista
					if (it->movingPoints > temp.movingPoints)
						it->movingPoints = temp.movingPoints;

				}
			}

			if (differentTile == true)
				moves->push_back(temp);

			getPossibleMoves(unit, tempMps, temp, moves);
		}
	}
	temp = temp2;

	//derecha
	if (moveRIGHTavailable(temp.destination))
	{
		temp.destination.column++; //estoy a la derecha
		terrains_d nextTerrain = getTerrain(temp.destination);
		temp.movingPoints += unit->getTerrainMC(nextTerrain);
		int tempMps = currMPs - temp.movingPoints;
		if (tempMps >= 0)
		{
			bool differentTile = true;
			for (list<moves_s>::iterator it = moves->begin(); it != moves->end(); it++)
			{
				if (it->destination == temp.destination)
				{
					differentTile = false; // ya existe el camino en la lista
					if (it->movingPoints > temp.movingPoints)
						it->movingPoints = temp.movingPoints;

				}
			}

			if (differentTile == true)
				moves->push_back(temp);

			getPossibleMoves(unit, tempMps, temp, moves);
		}

	}
	temp = temp2;
	//izquierda
	if (moveLEFTavailable(temp.destination))
	{
		temp.destination.column--; //estoy a la izquierda 
		terrains_d nextTerrain = getTerrain(temp.destination);
		temp.movingPoints += unit->getTerrainMC(nextTerrain);
		int tempMps = currMPs - temp.movingPoints;
		if (tempMps >= 0)
		{
			bool differentTile = true;
			for (list<moves_s>::iterator it = moves->begin(); it != moves->end(); it++)
			{
				if (it->destination == temp.destination)
				{
					differentTile = false; // ya existe el camino en la lista
					if (it->movingPoints > temp.movingPoints)
						it->movingPoints = temp.movingPoints;

				}
			}

			if (differentTile == true)
				moves->push_back(temp);

			getPossibleMoves(unit, tempMps, temp, moves);
		}
	}

}



unsigned int Map::getMoveMPS(Unit * unit, Position destination) {
	
	list<moves_s> MovesPossible;
	moves_s temp;
	temp.movingPoints = 0;
	temp.destination = unit->getPosition();

	unsigned int ans = MP_MAX;

	getPossibleMoves(unit, unit->getActualMP(), temp, &MovesPossible);

	for (list<moves_s>::iterator it = MovesPossible.begin(); it != MovesPossible.end(); it++)
	{
		if (it->destination == destination)
			ans = it->movingPoints;
	}

	return ans;
}


void Map::updateCP()
{
	for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
		for (unsigned int j = 0; j < BOARD_WIDTH; j++) {

			Position pos(i, j);

			if (IsBuildingOnTop(pos) && getBuilding(pos).BuildingUnderAttack() && IsUnitOnTop(pos) && getUnitTeam(pos) == team )
			{
				getBuildingPtr(pos)->captureBuilding(team, getUnit(pos).isReduced());
				if (getBuilding(pos).BuildingUnderAttack())
					getUnitPtr(pos)->setStatus(BLOCKED);
				
			}
		}
	}
}

void Map::endTurnUnits()
{
	for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
		for (unsigned int j = 0; j < BOARD_WIDTH; j++) {

			Position pos(i, j);
			if (IsUnitOnTop(pos))
			{
				getUnitPtr(pos)->endTurn();
			}
		}
	}
}


bool Map::enemyMove(Position WhereTo, Unit * unit)
{
	bool valid = false;

	if (IsBuildingOnTop(unit->getPosition()))
		getBuilding(unit->getPosition()).resetCapturePoints();

	unit->setMP(unit->getActualMP() - getMoveMPS(unit, WhereTo));

	changeUnitPos(unit, WhereTo);
	valid = true;

	if (unit->isItAPC())
	{
		((classAPC*)this)->ChangeUnitsPosition();
	}

	return valid;

}



