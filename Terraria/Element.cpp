#include "Element.h"


Element::Element()
{
}


Element::~Element()
{
}

void Element::render() {
	// TODO: foreach element, show it in a table
}

vector<Element*> Element::getCraftingMaterials() {
	return craftingMaterials;
}

float Element::getDamage()
{
	return damage;
}

int Element::getTileIndex()
{
	return tileIndex;
}