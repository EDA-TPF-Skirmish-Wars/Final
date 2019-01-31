#include "Position.h"

Position::Position()
{
	row = column = 0;
}

Position::Position(int row, int column)
{
	this->column = column;
	this->row = row;
}

bool Position::operator!=(Position pos)
{
	if (pos.row != row || pos.column != column)
		return true;
	else
		return false;
}
