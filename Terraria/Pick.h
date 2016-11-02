#include "Element.h"
class Pick :
	public Element
{
public:
	Pick();
	elementTypes getType();
	Element* clone() const { return new Pick(*this); }
};

