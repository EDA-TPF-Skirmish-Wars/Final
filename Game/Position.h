#ifndef POSITION_H
#define POSITION_H

class Position
{
public:
	Position();
	Position(int row, int column);
	bool operator!=(Position pos);

	int row;
	int column;
};
#endif