#include "Weapon.h"


Weapon::Weapon()
{
	tileIndex = 2;
	damage = 60;
	attSpeed = 1.2f;
	//craftingMaterials = 
}

int Weapon::getType()
{
	return 1;
}