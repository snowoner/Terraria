#include "Material.h"


Material::Material()
{
	tileIndex = 3;
	damage = 0;
	attSpeed = 1.f;
}

int Material::getType()
{
	return 1;
}