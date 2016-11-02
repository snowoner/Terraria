#include "Element.h"

void Element::add(int quantity)
{
	this->quantity += quantity;
}

void Element::consume(int quantity)
{
	this->quantity -= quantity;
}