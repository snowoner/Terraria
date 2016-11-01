#ifndef _ELEMENTFACTORY_INCLUDE
#define _ELEMENTFACTORY_INCLUDE

#include "SpriteArray.h"
#include "Element.h"
#include "Sprite.h"
#include "Weapon.h"
#include "Pick.h"
#include "Armor.h"
#include "Material.h"
#include "TileMap.h"

#define MAX_ITEMS_SHOWN 5
#define CRAFT_MATERIALS 4

class ElementFactory
{
public:
	ElementFactory(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram);

	struct craftMaterial{
		int type;
		int quantity;
	};

	void init();
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &minCoords);

	void setElementSelected(int selected);
	Element* getElementByIndex(int index);
	Element* getElementSelected();
	int getIndexElementSelected();

	void addElement(int type);
	void collectElement(int index);
	void consumeElement(Element *element, int quantity);
	void removeElement(Element *element);
	vector<pair<elementTypes, vector<craftMaterial>>> ElementFactory::listCraftItems();

	void setTileMap(TileMap *tileMap);
	void addMapMaterial(int type, glm::vec2 position);
	vector<pair<glm::ivec2*, int>> getMapMaterials();

	void craftElement(int index);
	vector<int> getMaterialsCanCraft() const { return itemsIndexToCraft; }
	vector<pair<elementTypes, vector<craftMaterial>>> getCraftingMaterials();
	vector<pair<elementTypes, vector<craftMaterial>>> craftintMaterials;
private:
	int getElementPosition(Element *element);
private:
	int updateTime;

	vector<Element*> elements;
	int selected;

	vector<pair<glm::ivec2*, int>> mapMaterials;
	vector<int> itemsIndexToCraft;
	TileMap *map;

};

#endif // _ELEMENTFACTORY_INCLUDE