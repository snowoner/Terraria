#include "Material.h"


Material::Material()
{
	damage = 0;
	attSpeed = 1.f;
}

int Material::getType()
{
	return 1;
}