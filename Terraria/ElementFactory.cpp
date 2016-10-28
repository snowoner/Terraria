#include "ElementFactory.h"

#define MAXUPDATE_TIME 1000.f/16.f

ElementFactory::ElementFactory(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram)
{
	elements.push_back(new Pick());
	elements.push_back(new Material());
	elements.push_back(new Weapon());
	elements.push_back(NULL);
	elements.push_back(NULL);

	updateTime = 0.f;

	//positionMapMaterials = new vector<glm::ivec2*>();
}

void ElementFactory::update(float deltaTime)
{
	updateTime += deltaTime;
	if (updateTime >= MAXUPDATE_TIME)
	{
		updateTime -= MAXUPDATE_TIME;
		for (glm::ivec2 *position : positionMapMaterials)
		{
			position->y += 2.f;
			map->collisionMoveDown(*position, glm::ivec2(32, 32), &(position->y));
		}
	}
}

// TODO: only render it when player change some weapon and at init
void ElementFactory::render()
{
	for (Element* element : elements)
		element->render();
}

void ElementFactory::setTileMap(TileMap *tileMap)
{
	map = tileMap;
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

int ElementFactory::getIndexElementSelected()
{
	return selected;
}

// TODO: need to collect element passing element
void ElementFactory::collectElement(int index)
{
	positionMapMaterials.erase(positionMapMaterials.begin() + index);

	bool found = false;
	int firstNull = -1;
	for (unsigned int i = 0; i < elements.size(); ++i){
		if (elements.at(i) == NULL && firstNull == -1) firstNull = i;
		if (dynamic_cast<Material*>(elements[i])) {
			elements[i]->add(1);
			found = true;
		}
	}

	if (!found)
	{
		if (firstNull != -1) elements[firstNull] = new Material();
		else elements.push_back(new Material());
	}
}

void ElementFactory::consumeElement(Element *element, int quantity)
{
	element->consume(quantity);
	if (element->getQuantity() == 0)
		this->removeElement(element);
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

// TODO: MapMaterials should have a paor (type, position)
void ElementFactory::addMapMaterial(int type, glm::vec2 position)
{
	glm::ivec2 *pos = new glm::ivec2(position);
	positionMapMaterials.push_back(pos);
}

vector<glm::ivec2*> ElementFactory::getMapMaterialsPosition()
{
	return positionMapMaterials;
}
