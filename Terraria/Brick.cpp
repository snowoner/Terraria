#include "Brick.h"
Brick::Brick()
{

}

Brick::Brick(int type, int hits)
{
	this->hits = hits;
	this->type = type;
}

void Brick::hit()
{
	hits--;
}

void Brick::setType(int type)
{
	this->type = type;
}
