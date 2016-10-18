#include "ElementFactory.h"


ElementFactory::ElementFactory()
{
	elements.push_back(new Pick());
	elements.push_back(new Material());
	elements.push_back(new Weapon());
	this->setElementSelected(0);
}


ElementFactory::~ElementFactory()
{
}


void ElementFactory::render()
{
	for (Element* element : elements)
		element->render();
}

Element* ElementFactory::addElement(int type)
{
	//elements.push_back(new Element());
	return elements.back();
}

void ElementFactory::setElementSelected(int selected)
{
	this->selected = selected;
}

Element* ElementFactory::getElementSelected()
{
	return elements.at(selected);
}

void ElementFactory::removeElement(Element *element)
{
	int pos = this->getElementPosition(element);
	elements.erase(elements.begin() + pos);
}

bool ElementFactory::craftElement(int type)
{
	vector<Element*> elementsNeed = elements[0]->getCraftingMaterials();

	for (Element* e : elementsNeed){
		if (std::find(elements.begin(), elements.end(), e) == elements.end()) return false;
		// Check if has material to do it?? or put materials to craft and craft materials searching which material to do?
	}
	return true;
}

int ElementFactory::getElementPosition(Element *element)
{
	return find(elements.begin(), elements.end(), element) - elements.begin();
}