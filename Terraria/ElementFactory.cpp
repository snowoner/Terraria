#include "ElementFactory.h"


#define SLOT_TILESIZEX 300
#define SLOT_TILESIZEY 56
#define SLOT_OFFSET 8
#define MAX_SLOT 5

#define POSINIT glm::vec2(5.f,5.f)

ElementFactory::ElementFactory(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram)
{
	elements.push_back(new Pick());
	elements.push_back(new Material());
	elements.push_back(new Weapon());
	elements.push_back(NULL);

	textGenerator = new TextureGenerator();
	textGenerator->init(&shaderProgram, minCoords, "images/glass.png",
		glm::vec2(1, 1), glm::ivec2(SLOT_TILESIZEX, SLOT_TILESIZEY),
		glm::ivec2(0,0), 0, true);

	textGeneratorSel = new TextureGenerator();
	textGeneratorSel->init(&shaderProgram, minCoords, "images/selected.png",
		glm::vec2(1, 1), glm::ivec2(50, 48),
		glm::ivec2(0, 0), 0);

	textGeneratorItems = new TextureGenerator();
	textGeneratorItems->init(&shaderProgram, minCoords, "images/items.png",
		glm::vec2(3, 1), glm::ivec2(48, 48), glm::ivec2(SLOT_TILESIZEX / MAX_SLOT, 0), 0);

	prepareArrays();

	this->setElementSelected(0);
}

void ElementFactory::prepareArrays()
{
	textGenerator->removeTiles();
	textGeneratorItems->removeTiles();

	vector<int> tiles(1,1);
	vector<int> tilesItems;
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i] != NULL)
			tilesItems.push_back(elements[i]->getTileIndex());
		else
			tilesItems.push_back(NULLTILE);
	}

	glm::vec2 position = POSINIT;

	textGenerator->addTiles(tiles, position);
	textGeneratorItems->addTiles(tilesItems, glm::vec2(position.x + SLOT_OFFSET / 2.f, position.y + SLOT_OFFSET / 2.f));

	textGenerator->prepareArrays();
	textGeneratorItems->prepareArrays();
}

// TODO: only render it when player change some weapon and at init
void ElementFactory::render()
{
	textGenerator->render();
	textGeneratorSel->render();
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

	textGeneratorSel->removeTiles();
	vector<int> tiles(1, 1);
	textGeneratorSel->addTiles(tiles, glm::vec2(POSINIT.x + SLOT_OFFSET / 2.f + (selected)*(48.f + SLOT_OFFSET * 1.5f), POSINIT.y + SLOT_OFFSET / 2.f));
	textGeneratorSel->prepareArrays();
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
	textGeneratorSel->setPosition(minCoords);
	textGeneratorItems->setPosition(minCoords);
}