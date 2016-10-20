#include "ElementFactory.h"


ElementFactory::ElementFactory(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram)
{
	elements.push_back(new Pick());
	elements.push_back(new Material());
	elements.push_back(new Weapon());
	this->setElementSelected(0);

	textGenerator = new TextureGenerator();
	textGenerator->init(&shaderProgram, minCoords, "images/glass.png",
		glm::vec2(1, 1),
		48, 48, 0, true);

	textGeneratorItems = new TextureGenerator();
	textGeneratorItems->init(&shaderProgram, minCoords, "images/items.png",
		glm::vec2(3, 1),
		48, 48, 0);

	prepareArrays();
}

void ElementFactory::prepareArrays()
{
	textGenerator->removeTiles();
	textGeneratorItems->removeTiles();

	glm::vec2 position = glm::vec2(5.f, 5.f);
	vector<int> tiles;
	vector<int> tilesItems;
	for (int i = 0; i < elements.size(); i++) {
		tiles.push_back(1);
		tilesItems.push_back(elements[i]->getTileIndex());
	}

	textGenerator->addTiles(tiles, position);
	textGeneratorItems->addTiles(tilesItems, position);

	textGenerator->prepareArrays();
	textGeneratorItems->prepareArrays();
}

// TODO: only render it when player change some weapon and at init
void ElementFactory::render()
{
	textGenerator->render();
	textGeneratorItems->render();
	for (Element* element : elements)
		element->render();
}

// TODO: implement add element by type
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
	if (pos != -1) elements.erase(elements.begin() + pos);
	prepareArrays();
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

void ElementFactory::setPosition(const glm::vec2 &minCoords)
{
	textGenerator->setPosition(minCoords);
	textGeneratorItems->setPosition(minCoords);
}