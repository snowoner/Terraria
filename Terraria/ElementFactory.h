#ifndef _ELEMENTFACTORY_INCLUDE
#define _ELEMENTFACTORY_INCLUDE

#include "Element.h"
#include "Sprite.h"
#include "Weapon.h"
#include "Pick.h"
#include "Material.h"


class ElementFactory
{
public:
	ElementFactory();
	~ElementFactory();

	void render();

	void setElementSelected(int selected);
	Element* getElementSelected();

	Element *addElement(int type);
	void removeElement(Element *element);
	bool craftElement(int type);
private:
	int getElementPosition(Element *element);

private:
	vector<Element*> elements;
	int selected;
};

#endif // _ELEMENTFACTORY_INCLUDE