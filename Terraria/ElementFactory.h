#ifndef _ELEMENTFACTORY_INCLUDE
#define _ELEMENTFACTORY_INCLUDE

#include "TextureGenerator.h"
#include "Element.h"
#include "Sprite.h"
#include "Weapon.h"
#include "Pick.h"
#include "Material.h"
#include "TileMap.h"

#define MAX_ITEMS_SHOWN 5
class ElementFactory
{
public:
	ElementFactory(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram);

	void update(float deltaTime);
	void render();

	void setPosition(const glm::vec2 &minCoords);

	void setElementSelected(int selected);
	Element* getElementByIndex(int index);
	Element* getElementSelected();
	int getIndexElementSelected();

	Element *addElement(int type);
	void collectElement(int index);
	void consumeElement(Element *element, int quantity);
	void removeElement(Element *element);
	bool craftElement(int type);

	void setTileMap(TileMap *tileMap);
	void addMapMaterial(int type, glm::vec2 position);
	vector<glm::ivec2*> getMapMaterialsPosition();
private:
	int getElementPosition(Element *element);
private:
	float updateTime;

	vector<Element*> elements;
	int selected;

	vector<glm::ivec2*> positionMapMaterials;
	TileMap *map;
};

#endif // _ELEMENTFACTORY_INCLUDE