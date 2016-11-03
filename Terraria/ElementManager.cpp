#include "ElementManager.h"
#include "Game.h"

#define SLOT_TILESIZEX 300
#define SLOT_TILESIZEY 56
#define SLOT_OFFSET 3
#define CRAFT_SIZE 24
#define EQUIP_SIZE 48
#define MAX_SLOT 5

#define POSINIT glm::vec2(5.f,5.f)
#define POS_CRAFTING_ITEMS (POSINIT + glm::vec2(SLOT_OFFSET, SLOT_OFFSET + 150))
#define POS_EQUIP_ITEMS (POSINIT + glm::vec2(Game::instance().getSize() - glm::ivec2(100,250)))
#define ITEM_BLOCKSIZE 56.f
#define ITEMSVEC glm::vec2(4,2)

enum spriteTypes {
	SLOTS, SLOTCRAFT, SLOTEQUIP, SELECTION, ITEMS, CRAFTING, EQUIP, MATERIALS,
	LAST_SPRITE
};

enum elementTiles{
	TILE_PICK = 5, TILE_WEAPON = 6, TILE_MATERIAL = 1, TILE_ARMOR = 7
};

ElementManager::ElementManager(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram)
{
	elementFactory = new ElementFactory(minCoords, shaderProgram);
	elementFactory->init();

	sprites = new SpriteArray*[LAST_SPRITE]();

	sprites[SLOTS] = new SpriteArray();
	sprites[SLOTS]->init(&shaderProgram, minCoords, "images/slots.png",
		glm::vec2(1, 1), glm::ivec2(SLOT_TILESIZEX, SLOT_TILESIZEY), glm::ivec2(0, 0), 0, true);

	sprites[SLOTCRAFT] = new SpriteArray();
	sprites[SLOTCRAFT]->init(&shaderProgram, minCoords, "images/slot.png",
		glm::vec2(1, 1), glm::ivec2(CRAFT_SIZE, CRAFT_SIZE), glm::ivec2(0, CRAFT_SIZE + SLOT_OFFSET), 0, true);

	sprites[SLOTEQUIP] = new SpriteArray();
	sprites[SLOTEQUIP]->init(&shaderProgram, minCoords, "images/slot.png",
		glm::vec2(1, 1), glm::ivec2(EQUIP_SIZE, EQUIP_SIZE), glm::ivec2(0, EQUIP_SIZE + SLOT_OFFSET), 0, true);

	sprites[SELECTION] = new SpriteArray();
	sprites[SELECTION]->init(&shaderProgram, minCoords, "images/selected.png",
		glm::vec2(1, 1), glm::ivec2(ITEM_BLOCKSIZE, ITEM_BLOCKSIZE), glm::ivec2(0, 0));

	sprites[ITEMS] = new SpriteArray();
	sprites[ITEMS]->init(&shaderProgram, minCoords, "images/items.png",
		ITEMSVEC, glm::ivec2(ITEM_BLOCKSIZE - SLOT_OFFSET * 2, ITEM_BLOCKSIZE - SLOT_OFFSET * 2), glm::ivec2(SLOT_TILESIZEX / MAX_SLOT, 0));

	sprites[CRAFTING] = new SpriteArray();
	sprites[CRAFTING]->init(&shaderProgram, minCoords, "images/items.png",
		ITEMSVEC, glm::ivec2(CRAFT_SIZE, CRAFT_SIZE), glm::ivec2(0, CRAFT_SIZE + SLOT_OFFSET));

	sprites[EQUIP] = new SpriteArray();
	sprites[EQUIP]->init(&shaderProgram, minCoords, "images/items.png",
		ITEMSVEC, glm::ivec2(EQUIP_SIZE, EQUIP_SIZE), glm::ivec2(0, EQUIP_SIZE + SLOT_OFFSET));

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
	text->init(&shaderProgram, minCoords, 1);
	prepareSpritesItems();

	setElementIndexSelected(0);
}

void ElementManager::prepareSpritesItems()
{
	sprites[ITEMS]->removeTiles();
	text->removeTiles();

	vector<int> tilesItems;
	vector<int> tilesCraftItems;
	vector<int> tilesText;

	for (int i = 0; i < MAX_ITEMS_SHOWN; i++) {
		Element *element = elementFactory->getElementByIndex(i);
		if (element != NULL)
		{
			tilesItems.push_back(element->getTileIndex());
			string textQuantity = std::to_string(element->getQuantity());
			text->addText(textQuantity, glm::vec2((ITEM_BLOCKSIZE + SLOT_OFFSET*1.33f)*i + ITEM_BLOCKSIZE - 16.f*textQuantity.length(), ITEM_BLOCKSIZE - 10.f));
		}
		else tilesItems.push_back(NULLTILE);
	}

	sprites[CRAFTING]->removeTiles();
	sprites[SLOTCRAFT]->removeTiles();
	tilesCraftItems.clear();
	vector<pair<elementTypes, vector<ElementFactory::craftMaterial>>> craftMaterials = elementFactory->listCraftItems();
	for (unsigned int i = 0; i < craftMaterials.size(); i++) {
		pair<elementTypes, vector<ElementFactory::craftMaterial>> craftElement = craftMaterials.at(i);
		for (ElementFactory::craftMaterial elemMaterials : craftElement.second)
			tilesCraftItems.push_back(getTileIndex(craftElement.first));
	}
	if (tilesCraftItems.size() > 0) {
		sprites[SLOTCRAFT]->addTiles(vector<int>(tilesCraftItems.size(), 1), POS_CRAFTING_ITEMS);
		sprites[SLOTCRAFT]->prepareArrays();
		sprites[CRAFTING]->addTiles(tilesCraftItems, POS_CRAFTING_ITEMS);
		sprites[CRAFTING]->prepareArrays();
	}

	text->prepareText();
	sprites[ITEMS]->addTiles(tilesItems, POSINIT + glm::vec2(SLOT_OFFSET, SLOT_OFFSET));
	sprites[ITEMS]->prepareArrays();
}

void ElementManager::prepareSpritesMaterials() 
{
	vector<pair<glm::ivec2*, int>> positionMapMaterials = elementFactory->getMapMaterials();
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

void ElementManager::prepareSpritesEquip()
{
	sprites[SLOTEQUIP]->removeTiles();
	sprites[EQUIP]->removeTiles();

	vector<int> tilesEquip;
	vector<Element*> equip = elementFactory->getEquip();
	for (unsigned int i = 0; i < equip.size(); i++) {
		tilesEquip.push_back(getTileIndex(equip.at(i)->getType()));
	}

	if (tilesEquip.size() > 0) {
		sprites[SLOTEQUIP]->addTiles(vector<int>(tilesEquip.size(), 1), glm::vec2(POS_EQUIP_ITEMS));
		sprites[SLOTEQUIP]->prepareArrays();
		sprites[EQUIP]->addTiles(tilesEquip, glm::vec2(POS_EQUIP_ITEMS));
		sprites[EQUIP]->prepareArrays();
	}
}

void ElementManager::update(int deltaTime)
{
	elementFactory->update(deltaTime);

	prepareSpritesMaterials();
}

void ElementManager::render()
{
	sprites[SLOTS]->render();
	sprites[SELECTION]->render();
	sprites[ITEMS]->render();
	if (elementFactory->getMaterialsCanCraft().size() > 0)
	{
		sprites[SLOTCRAFT]->render();
		sprites[CRAFTING]->render();
	}
	if (elementFactory->getMapMaterials().size() > 0) 
		sprites[MATERIALS]->render();
	if (showEquip && elementFactory->getEquip().size()) {
		sprites[SLOTEQUIP]->render();
		sprites[EQUIP]->render();
	}
	text->render();
}

void ElementManager::setPosition(const glm::vec2 &minCoords)
{
	sprites[SLOTS]->setPosition(minCoords);
	sprites[SELECTION]->setPosition(minCoords + glm::vec2((elementFactory->getElementIndexSelected())*(ITEM_BLOCKSIZE + SLOT_OFFSET*1.33f), 0.f));
	sprites[ITEMS]->setPosition(minCoords);
	sprites[SLOTCRAFT]->setPosition(minCoords);
	sprites[CRAFTING]->setPosition(minCoords);
	sprites[SLOTEQUIP]->setPosition(minCoords);
	sprites[EQUIP]->setPosition(minCoords);
	text->setPosition(minCoords);
}

void ElementManager::setTileMap(TileMap *tileMap)
{
	elementFactory->setTileMap(tileMap);
}

void ElementManager::setElementIndexSelected(int selected)
{
	elementFactory->setElementIndexSelected(selected);
}

Element* ElementManager::getElementSelected()
{
	return elementFactory->getElementSelected();
}

void ElementManager::collectElement(int index)
{
	elementFactory->collectElement(index);
	prepareSpritesItems();
	prepareSpritesMaterials();
}

void ElementManager::removeElement(Element *element)
{
	consumeElement(element, 1);
}

void ElementManager::consumeElement(Element *element, int quantity)
{
	elementFactory->consumeElement(element, quantity);
	prepareSpritesItems();
}

float ElementManager::getTotalDefenseEquip()
{
	float totalDefense = 0.f;
	vector<Element*> equip = elementFactory->getEquip();
	for (unsigned int i = 0; i < equip.size(); ++i) totalDefense += equip[i]->getDefense();
	return totalDefense;
}

void ElementManager::equipElement(int index)
{
	if (elementFactory->equipElement(elementFactory->getElementByIndex(index)))
	{
		prepareSpritesItems();
		prepareSpritesEquip();
	}
}

void ElementManager::unequipElement(int index)
{
	elementFactory->unequipElement(index);
	prepareSpritesItems();
	prepareSpritesEquip();
}

void ElementManager::craftElement(int index)
{
	elementFactory->craftElement(index);
	prepareSpritesItems();
	prepareSpritesMaterials();
}

void ElementManager::addElementMaterial(int type, glm::ivec2 position)
{
	elementFactory->addMapMaterial(type, position);
}

vector<pair<glm::ivec2*, int>> ElementManager::getMapMaterials()
{
	return elementFactory->getMapMaterials();
}

int ElementManager::getElement(glm::vec2 position)
{
	for (int i = 0; i < elementFactory->getItemsSize(); ++i)
	if ((ITEM_BLOCKSIZE + SLOT_OFFSET)*i < position.x
		&& position.x < (ITEM_BLOCKSIZE + SLOT_OFFSET)*i + ITEM_BLOCKSIZE
		&& POSINIT.y < position.y
		&& position.y < POSINIT.y + ITEM_BLOCKSIZE)
		if(elementFactory->getElementByIndex(i) != NULL) return i;
	return -1;
}

int ElementManager::getCraftingElement(glm::vec2 position)
{
	return getElementIndexPosition(elementFactory->getCraftingMaterials().size(), POS_CRAFTING_ITEMS, position, CRAFT_SIZE);
}

int ElementManager::getEquipElement(glm::vec2 position)
{
	return getElementIndexPosition(elementFactory->getEquip().size(), POS_EQUIP_ITEMS, position, EQUIP_SIZE);
}

int ElementManager::getElementIndexPosition(unsigned int size, glm::vec2 posElements, glm::vec2 position, int boxSize)
{
	for (unsigned int i = 0; i < size; ++i)
	if (posElements.x < position.x && position.x < posElements.x + boxSize
		&& posElements.y + (boxSize + SLOT_OFFSET)*i < position.y
		&& position.y < posElements.y + (boxSize + SLOT_OFFSET)*i + boxSize)
		return i;
	return -1;
}

int ElementManager::getTileIndex(elementTypes type)
{
	switch (type)
	{
	case WEAPON:
		return TILE_WEAPON;
		break;
	case PICK:
		return TILE_PICK;
		break;
	case ARMOR:
		return TILE_ARMOR;
		break;
	default:
		return -1;
	}
}