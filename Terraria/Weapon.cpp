#include "Weapon.h"

Weapon::Weapon()
{
	tileIndex = 6;
	damage = 60;
	attSpeed = 1.2f;
	quantity = 1; 
	equip = false;
}

elementTypes Weapon::getType()
{
	return WEAPON;
}