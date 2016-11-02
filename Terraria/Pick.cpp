#include "Pick.h"


Pick::Pick()
{
	tileIndex = 5;
	damage = 5;
	attSpeed = 1.f;
	quantity = 1;
	equip = false;
}

elementTypes Pick::getType(){
	return PICK;
}