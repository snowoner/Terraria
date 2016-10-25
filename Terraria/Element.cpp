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

void Element::add(int quantity)
{
	this->quantity += quantity;
}

void Element::consume(int quantity)
{
	this->quantity -= quantity;
}