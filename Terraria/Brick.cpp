#include "Brick.h"
Brick::Brick()
{

}

Brick::Brick(int type, int tile, int hits, bool destroyable)
{
	this->hits = hits;
	this->type = type;
	this->tile = tile;
	this->destroyable = destroyable;
}

void Brick::hit()
{
	hits--;
}

void Brick::setTile(int tile)
{
	this->tile= tile;
}