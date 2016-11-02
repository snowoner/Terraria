#include "Element.h"
class Material :
	public Element
{
public:
	Material(int type);
	elementTypes getType();
	Element* clone() const { return new Material(*this); }
};

