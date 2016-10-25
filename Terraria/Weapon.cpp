#include "Weapon.h"


Weapon::Weapon()
{
	tileIndex = 2;
	damage = 60;
	attSpeed = 1.2f;
	quantity = 1;
	//craftingMaterials = 
}

int Weapon::getType()
{
	return 1;
}