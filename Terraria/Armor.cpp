#include "Armor.h"

Armor::Armor()
{
	tileIndex = 7;
	damage = 0;
	attSpeed = 1.f;
	quantity = 1;
}

int Armor::getType(){
	return ARMOR;
}