#ifndef _ELEMENTMANAGER_INCLUDE
#define _ELEMENTMANAGER_INCLUDE

#include "ElementFactory.h"	
#include "Text.h"

class ElementManager
{
public:
	ElementManager(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram);
	
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &minCoords);
	void setTileMap(TileMap *tileMap);

	void setElementIndexSelected(int selected);
	Element* getElementSelected();

	void collectElement(int index);
	void removeElement(Element *element);
	void consumeElement(Element *element, int quantity);

	float getTotalDefenseEquip();
	void equipElement(int index);
	void unequipElement(int index);
	void craftElement(int index);

	void addElementMaterial(int type, glm::ivec2 position);
	vector<pair<glm::ivec2*, int>> getMapMaterials();

	int getElement(glm::vec2 position);
	int getCraftingElement(const glm::vec2 position);
	int getEquipElement(const glm::vec2 position);
private:
	void prepareSpritesItems();
	void prepareSpritesMaterials(); 
	void prepareSpritesEquip();

	int getElementIndexPosition(unsigned int size, glm::vec2 posElements, glm::vec2 position, int boxSize);

	int getTileIndex(elementTypes type);
private:
	SpriteArray **sprites;
	ElementFactory *elementFactory;
	Text *text;

	bool showEquip;
};

#endif