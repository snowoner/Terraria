#include "Element.h"
class Armor :
	public Element
{
public:
	Armor();
	elementTypes getType();
	Element* clone() const { return new Armor(*this); }
};

