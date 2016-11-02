#include "Armor.h"

Armor::Armor()
{
	tileIndex = 7;
	damage = 0;
	attSpeed = 1.f;
	quantity = 1;
	equip = true;
}

elementTypes Armor::getType(){
	return ARMOR;
}