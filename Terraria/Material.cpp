#include "Material.h"


Material::Material(int type)
{
	tileIndex = type;
	quantity = 1;
	damage = 0;
	attSpeed = 1.f;
}

int Material::getType()
{
	return MATERIAL;
}