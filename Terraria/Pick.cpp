#include "Pick.h"


Pick::Pick()
{
	tileIndex = 1;
	damage = 5;
	attSpeed = 1.f;
}

int Pick::getType(){
	return 1;
}