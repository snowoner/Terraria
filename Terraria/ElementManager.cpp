#include "ElementManager.h"

#define SLOT_TILESIZEX 300
#define SLOT_TILESIZEY 56
#define SLOT_OFFSET 8
#define MAX_SLOT 5

#define POSINIT glm::vec2(5.f,5.f)


ElementManager::ElementManager(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram)
{
	elementFactory = new ElementFactory(minCoords, shaderProgram);

	textGenerator = new TextureGenerator();
	textGenerator->init(&shaderProgram, minCoords, "images/glass.png",
		glm::vec2(1, 1), glm::ivec2(SLOT_TILESIZEX, SLOT_TILESIZEY),
		glm::ivec2(0, 0), 0, true);

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

void ElementManager::prepareArrays()
{
	textGenerator->removeTiles();
	textGeneratorItems->removeTiles();

	vector<int> tilesItems;
	for (int i = 0; i < MAX_ITEMS_SHOWN; i++) {
		Element *element = elementFactory->getElementByIndex(i);
		if (element != NULL)
			tilesItems.push_back(element->getTileIndex());
		else
			tilesItems.push_back(NULLTILE);
	}

	glm::vec2 position = POSINIT;

	textGenerator->addTiles(vector<int>(1, 1), position);
	textGeneratorItems->addTiles(tilesItems, glm::vec2(position.x + SLOT_OFFSET / 2.f, position.y + SLOT_OFFSET / 2.f));

	textGenerator->prepareArrays();
	textGeneratorItems->prepareArrays();
}

// TODO: only render it when player change some weapon and at init
void ElementManager::render()
{
	textGenerator->render();
	textGeneratorSel->render();
	textGeneratorItems->render();

	elementFactory->render();
}

// TODO: implement add element by type
Element* ElementManager::addElement(int type)
{
	return elementFactory->addElement(type);
}

void ElementManager::setElementSelected(int selected)
{
	elementFactory->setElementSelected(selected);
	textGeneratorSel->removeTiles();
	textGeneratorSel->addTiles(vector<int>(1,1), glm::vec2(POSINIT.x + SLOT_OFFSET / 2.f + (selected)*(48.f + SLOT_OFFSET * 1.5f), POSINIT.y + SLOT_OFFSET / 2.f));
	textGeneratorSel->prepareArrays();
}

Element* ElementManager::getElementSelected()
{
	return elementFactory->getElementSelected();
}

void ElementManager::removeElement(Element *element)
{
	elementFactory->removeElement(element);
	prepareArrays();
}

bool ElementManager::craftElement(int type)
{
	return elementFactory->craftElement(type);
}

void ElementManager::setPosition(const glm::vec2 &minCoords)
{
	textGenerator->setPosition(minCoords);
	textGeneratorSel->setPosition(minCoords);
	textGeneratorItems->setPosition(minCoords);
}