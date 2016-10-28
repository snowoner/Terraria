#ifndef _ELEMENTMANAGER_INCLUDE
#define _ELEMENTMANAGER_INCLUDE

#include "ElementFactory.h"	
#include "Text.h"

class ElementManager
{
public:
	ElementManager(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram);
	
	void update(float deltaTime);
	void render();

	void setPosition(const glm::vec2 &minCoords);

	void setElementSelected(int selected);
	Element* getElementSelected();

	Element *addElement(int type);
	void consumeElement(Element *element, int quantity);
	void removeElement(Element *element);
	bool craftElement(int type);

	void addElementMaterial(int type, glm::ivec2 position);
	void materialsMovements();
	vector<glm::ivec2*> getMapMaterialsPosition();

	void collectElement(int index);

	void setTileMap(TileMap *tileMap);
private:
	void prepareSpritesItems();
	void prepareSpritesMaterials();
private:
	SpriteArray **sprites;
	ElementFactory *elementFactory;
	Text *text;
};

#endif