#pragma once

class Position
{
public:
	Position();
	Position(int row, int column);
	bool operator!=(Position pos);

	int row;
	int column;
};