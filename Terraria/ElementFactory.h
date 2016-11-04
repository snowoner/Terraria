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

#define MAX_ITEMS_SHOWN 10
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

	void setPosition(const glm::vec2 &minCoords);
	void setTileMap(TileMap *tileMap);

	void addElement(int type);
	void addElement(Element *element);
	Element* getElementByIndex(int index);

	Element* getElementSelected();
	void setElementIndexSelected(int selected);
	int getElementIndexSelected();

	void collectElement(int index);
	void removeElement(Element *element);
	void consumeElement(Element *element, int quantity);
	bool equipElement(Element *element);
	void unequipElement(int index);

	void craftElement(int index);
	vector<int> getMaterialsCanCraft() const { return itemsIndexToCraft; }
	vector<pair<elementTypes, vector<craftMaterial>>> getCraftingMaterials();
	vector<pair<elementTypes, vector<craftMaterial>>> ElementFactory::listCraftItems();

	void addMapMaterial(int type, glm::vec2 position);
	vector<pair<glm::ivec2*, int>> getMapMaterials();

	int getItemsSize() const { return elements.size(); }
	vector<Element*> getEquip() const { return equip; }
private:
	int getElementPosition(Element *element);
private:
	int updateTime;
	TileMap *map;

	vector<Element*> elements;
	int selected;

	vector<pair<glm::ivec2*, int>> mapMaterials;
	vector<Element*> equip;
	vector<int> itemsIndexToCraft;
	vector<pair<elementTypes, vector<craftMaterial>>> craftintMaterials;
};

#endif // _ELEMENTFACTORY_INCLUDE