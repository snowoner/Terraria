#include "ElementManager.h"
#include "Game.h"

#define SLOT_TILESIZEX 300
#define SLOT_TILESIZEY 56
#define SLOT_OFFSET 3
#define MAX_SLOT 5

#define POSINIT glm::vec2(5.f,5.f)

#define ITEM_BLOCKSIZE 56.f
#define ITEMSVEC glm::vec2(4,2)

enum spriteTypes {
	SLOTS, SELECTION, ITEMS, MATERIALS
};

ElementManager::ElementManager(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram)
{
	elementFactory = new ElementFactory(minCoords, shaderProgram);

	sprites = new SpriteArray*[sizeof(spriteTypes)]();

	sprites[SLOTS] = new SpriteArray();
	sprites[SLOTS]->init(&shaderProgram, minCoords, "images/slots.png",
		glm::vec2(1, 1), glm::ivec2(SLOT_TILESIZEX, SLOT_TILESIZEY), glm::ivec2(0, 0), 0, true);

	sprites[SELECTION] = new SpriteArray();
	sprites[SELECTION]->init(&shaderProgram, minCoords, "images/selected.png",
		glm::vec2(1, 1), glm::ivec2(ITEM_BLOCKSIZE, ITEM_BLOCKSIZE), glm::ivec2(0, 0));

	sprites[ITEMS] = new SpriteArray();
	sprites[ITEMS]->init(&shaderProgram, minCoords, "images/items.png",
		ITEMSVEC, glm::ivec2(ITEM_BLOCKSIZE - SLOT_OFFSET * 2, ITEM_BLOCKSIZE - SLOT_OFFSET * 2), glm::ivec2(SLOT_TILESIZEX / MAX_SLOT, 0));

	sprites[MATERIALS] = new SpriteArray();
	sprites[MATERIALS]->init(&shaderProgram, minCoords, "images/items.png",
		ITEMSVEC, glm::ivec2(32, 32), glm::ivec2(0, 0));

	sprites[SLOTS]->removeTiles();
	sprites[SLOTS]->addTiles(vector<int>(1, 1), POSINIT);
	sprites[SLOTS]->prepareArrays();
	sprites[SELECTION]->removeTiles();
	sprites[SELECTION]->addTiles(vector<int>(1, 1), POSINIT);
	sprites[SELECTION]->prepareArrays();
	

	text = new Text();
	text->init(shaderProgram, minCoords, 1);
	prepareSpritesItems();

	setElementSelected(0);
}

void ElementManager::prepareSpritesItems()
{
	sprites[ITEMS]->removeTiles();
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

	sprites[ITEMS]->addTiles(tilesItems, POSINIT + glm::vec2(SLOT_OFFSET, SLOT_OFFSET));
	sprites[ITEMS]->prepareArrays();
}

void ElementManager::prepareSpritesMaterials() {

	vector<pair<glm::ivec2*,int>> positionMapMaterials = elementFactory->getMapMaterials();
	if (positionMapMaterials.size() > 0)
	{
		sprites[MATERIALS]->removeTiles();

		for (unsigned int i = 0; i < positionMapMaterials.size(); ++i){
			glm::ivec2* pos = positionMapMaterials[i].first;
			sprites[MATERIALS]->addTiles(vector<int>(1, positionMapMaterials[i].second), glm::vec2(*pos));
		}
		sprites[MATERIALS]->prepareArrays();
	}
}

void ElementManager::update(int deltaTime)
{
	bool found = false;
	int keyPressed = 1;
	while (keyPressed <= MAX_ITEMS_SHOWN && !found)
	{
		if (Game::instance().getKey(keyPressed + '0')) found = true;
		else keyPressed++;
	}
	if (found) setElementSelected(keyPressed - 1);


	elementFactory->update(deltaTime);

	prepareSpritesMaterials();
}

// TODO: only render it when player change some weapon and at init
void ElementManager::render()
{
	for (unsigned int i = 0; i < sizeof(sprites)-1; ++i)
		sprites[i]->render();
	if (elementFactory->getMapMaterials().size() > 0) sprites[MATERIALS]->render();
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
	prepareSpritesItems();
}

void ElementManager::removeElement(Element *element)
{
	elementFactory->removeElement(element);
	prepareSpritesItems();
}

bool ElementManager::craftElement(int type)
{
	return elementFactory->craftElement(type);
}

void ElementManager::setPosition(const glm::vec2 &minCoords)
{
	sprites[SLOTS]->setPosition(minCoords);
	sprites[SELECTION]->setPosition(minCoords + glm::vec2((elementFactory->getIndexElementSelected())*(ITEM_BLOCKSIZE + SLOT_OFFSET*1.33f), 0.f));
	sprites[ITEMS]->setPosition(minCoords);
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
	prepareSpritesItems();
	prepareSpritesMaterials();
}

vector<pair<glm::ivec2*,int>> ElementManager::getMapMaterials()
{
	return elementFactory->getMapMaterials();
}