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

	void setElementSelected(int selected);
	Element* getElementSelected();

	void consumeElement(Element *element, int quantity);
	void removeElement(Element *element);
	void craftElement(int index);

	void addElementMaterial(int type, glm::ivec2 position);
	void materialsMovements();
	vector<pair<glm::ivec2*, int>> getMapMaterials();
	int ElementManager::getCraftingElement(const glm::vec2 position);

	void collectElement(int index);

	void setTileMap(TileMap *tileMap);
private:
	void prepareSpritesItems();
	void prepareSpritesMaterials();

	int getTileIndex(elementTypes type);
private:
	SpriteArray **sprites;
	ElementFactory *elementFactory;
	Text *text;
};

#endif