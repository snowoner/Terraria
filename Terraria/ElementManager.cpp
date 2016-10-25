#include "ElementManager.h"

#define SLOT_TILESIZEX 300
#define SLOT_TILESIZEY 56
#define SLOT_OFFSET 3
#define MAX_SLOT 5

#define POSINIT glm::vec2(5.f,5.f)

#define ITEM_BLOCKSIZE 56.f

enum textGenerators {
	SLOTS, SELECTION, ITEMS, MATERIALS
};

ElementManager::ElementManager(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram)
{
	elementFactory = new ElementFactory(minCoords, shaderProgram);

	textGenerator = new TextureGenerator*[sizeof(textGenerators)]();

	textGenerator[SLOTS] = new TextureGenerator();
	textGenerator[SLOTS]->init(&shaderProgram, minCoords, "images/slots.png",
		glm::vec2(1, 1), glm::ivec2(SLOT_TILESIZEX, SLOT_TILESIZEY), glm::ivec2(0, 0), 0, true);

	textGenerator[SELECTION] = new TextureGenerator();
	textGenerator[SELECTION]->init(&shaderProgram, minCoords, "images/selected.png",
		glm::vec2(1, 1), glm::ivec2(ITEM_BLOCKSIZE, ITEM_BLOCKSIZE), glm::ivec2(0, 0));

	textGenerator[ITEMS] = new TextureGenerator();
	textGenerator[ITEMS]->init(&shaderProgram, minCoords, "images/items.png",
		glm::vec2(3, 1), glm::ivec2(ITEM_BLOCKSIZE - SLOT_OFFSET * 2, ITEM_BLOCKSIZE - SLOT_OFFSET * 2), glm::ivec2(SLOT_TILESIZEX / MAX_SLOT, 0));

	textGenerator[MATERIALS] = new TextureGenerator();
	textGenerator[MATERIALS]->init(&shaderProgram, minCoords, "images/items.png",
		glm::vec2(3, 1), glm::ivec2(32, 32), glm::ivec2(0, 0));

	textGenerator[SLOTS]->removeTiles();
	textGenerator[SLOTS]->addTiles(vector<int>(1, 1), POSINIT);
	textGenerator[SLOTS]->prepareArrays();
	textGenerator[SELECTION]->removeTiles();
	textGenerator[SELECTION]->addTiles(vector<int>(1, 1), POSINIT);
	textGenerator[SELECTION]->prepareArrays();
	

	text = new Text();
	text->init(shaderProgram, minCoords, 1);
	prepareTextGeneratorItems();

	setElementSelected(0);
}

void ElementManager::prepareTextGeneratorItems()
{
	textGenerator[ITEMS]->removeTiles();
	text->removeTiles();

	vector<int> tilesItems;
	vector<int> tilesText;
	for (int i = 0; i < MAX_ITEMS_SHOWN; i++) {
		Element *element = elementFactory->getElementByIndex(i);
		if (element != NULL)
		{
			tilesItems.push_back(element->getTileIndex());
			string textQuantity = std::to_string(element->getQuantity());
			text->addText(textQuantity, glm::vec2((ITEM_BLOCKSIZE + SLOT_OFFSET*1.33f)*i + ITEM_BLOCKSIZE - 8.f*textQuantity.length(), ITEM_BLOCKSIZE - 5.f));
		}
		else{
			tilesItems.push_back(NULLTILE);
		}		
	}

	text->prepareText();

	textGenerator[ITEMS]->addTiles(tilesItems, POSINIT + glm::vec2(SLOT_OFFSET, SLOT_OFFSET));
	textGenerator[ITEMS]->prepareArrays();
}

void ElementManager::update(float deltaTime)
{
	elementFactory->update(deltaTime);

	vector<glm::ivec2*> positionMapMaterials = elementFactory->getMapMaterialsPosition();
	if (positionMapMaterials.size() > 0)
	{
		textGenerator[MATERIALS]->removeTiles();

		for (unsigned int i = 0; i < positionMapMaterials.size(); ++i){
			glm::ivec2* pos = positionMapMaterials[i];
			textGenerator[MATERIALS]->addTiles(vector<int>(1, 3), glm::vec2(*pos));
		}
		textGenerator[MATERIALS]->prepareArrays();
	}
}

// TODO: only render it when player change some weapon and at init
void ElementManager::render()
{
	for (unsigned int i = 0; i < sizeof(textGenerators)-1; ++i)
		textGenerator[i]->render();
	if (elementFactory->getMapMaterialsPosition().size() > 0) textGenerator[MATERIALS]->render();
	text->render();
	elementFactory->render();
}

void ElementManager::setTileMap(TileMap *tileMap)
{
	elementFactory->setTileMap(tileMap);
}

// TODO: implement add element by type
Element* ElementManager::addElement(int type)
{
	return elementFactory->addElement(type);
}

void ElementManager::setElementSelected(int selected)
{
	elementFactory->setElementSelected(selected);
}

Element* ElementManager::getElementSelected()
{
	return elementFactory->getElementSelected();
}

void ElementManager::consumeElement(Element *element, int quantity)
{
	elementFactory->consumeElement(element, quantity);
	if (elementFactory->getElementSelected() == NULL)
		prepareTextGeneratorItems();
}

void ElementManager::removeElement(Element *element)
{
	elementFactory->removeElement(element);
	prepareTextGeneratorItems();
}

bool ElementManager::craftElement(int type)
{
	return elementFactory->craftElement(type);
}

void ElementManager::setPosition(const glm::vec2 &minCoords)
{
	textGenerator[SLOTS]->setPosition(minCoords);
	textGenerator[SELECTION]->setPosition(minCoords + glm::vec2((elementFactory->getIndexElementSelected())*(ITEM_BLOCKSIZE + SLOT_OFFSET*1.33f), 0.f));
	textGenerator[ITEMS]->setPosition(minCoords);
	text->setPosition(minCoords);
	//textGenerator[MATERIALS]->setPosition(minCoords);
}

void ElementManager::addElementMaterial(int type, glm::ivec2 position)
{
	elementFactory->addMapMaterial(type, position);
}

// TODO: we need to pass Element and not index
void ElementManager::collectElement(int index)
{
	elementFactory->collectElement(index);
	prepareTextGeneratorItems();
	textGenerator[MATERIALS]->removeTiles();
	vector<glm::ivec2*> positionMapMaterials = elementFactory->getMapMaterialsPosition();
	if (positionMapMaterials.size() > 0)
	{
		for (unsigned int i = 0; i < positionMapMaterials.size(); ++i){
			glm::ivec2* pos = positionMapMaterials[i];
			textGenerator[MATERIALS]->addTiles(vector<int>(1, 3), glm::vec2(*pos));
		}
		textGenerator[MATERIALS]->prepareArrays();
	}
}

vector<glm::ivec2*> ElementManager::getMapMaterialsPosition()
{
	return elementFactory->getMapMaterialsPosition();
}