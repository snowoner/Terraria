#include "ElementFactory.h"


ElementFactory::ElementFactory()
{

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
	elements.push_back(new Element());
	return elements.back();
}

void ElementFactory::removeElement(int n) {
	int i = 0;
	elements.erase(elements.begin() + n - 1);
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