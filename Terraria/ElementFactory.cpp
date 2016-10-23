#include "ElementFactory.h"




ElementFactory::ElementFactory(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram)
{
	elements.push_back(new Pick());
	elements.push_back(new Material());
	elements.push_back(new Weapon());
	elements.push_back(NULL);
	elements.push_back(NULL);
}

// TODO: only render it when player change some weapon and at init
void ElementFactory::render()
{
	for (Element* element : elements)
		element->render();
}

// TODO: implement add element by type
Element* ElementFactory::addElement(int type)
{
	//elements.push_back(new Element());
	return elements.back();
}


Element* ElementFactory::getElementByIndex(int index)
{
	return elements[index];
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
	if (pos != -1)
	{
		delete elements[pos];
		elements[pos] = NULL;
	}
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