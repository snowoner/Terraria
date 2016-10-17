#ifndef _ELEMENTFACTORY_INCLUDE
#define _ELEMENTFACTORY_INCLUDE

#include "Element.h"
#include "Sprite.h"


class ElementFactory
{
public:
	ElementFactory();
	~ElementFactory();

	void render();


	Element *addElement(int type);
	void removeElement(int n);
	bool craftElement(int type);

private:
	vector<Element*> elements;
};

#endif // _ELEMENTFACTORY_INCLUDE