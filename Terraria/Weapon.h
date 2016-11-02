#include "Element.h"
class Weapon :
	public Element
{
public:
	Weapon();
	elementTypes getType();
	Element* clone() const { return new Weapon(*this); }
};

