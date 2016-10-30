#include "Weapon.h"


Weapon::Weapon()
{
	tileIndex = 6;
	damage = 60;
	attSpeed = 1.2f;
	quantity = 1;
	//craftingMaterials = 
}

int Weapon::getType()
{
	return WEAPON;
}