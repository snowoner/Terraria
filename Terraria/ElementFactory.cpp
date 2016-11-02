#include "ElementFactory.h"

#define MAXUPDATE_TIME 1000/16

ElementFactory::ElementFactory(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram)
{
	elements.push_back(new Pick());
	elements.push_back(new Weapon());
	elements.push_back(NULL);
	elements.push_back(NULL);
	elements.push_back(NULL);

	updateTime = 0;
}

void ElementFactory::init()
{
	vector<craftMaterial> craftWeapon = vector<craftMaterial>(
	{
		{ 1, 1 }
	});
	vector<craftMaterial> craftPick = vector<craftMaterial>(
	{
		{ 1, 1 }
	});
	vector<craftMaterial> craftArmor = vector<craftMaterial>(
	{
		{ 2, 3 }
	});
	craftintMaterials.push_back(pair<elementTypes, vector<craftMaterial>>(WEAPON, craftWeapon));
	craftintMaterials.push_back(pair<elementTypes, vector<craftMaterial>>(PICK, craftPick));
	craftintMaterials.push_back(pair<elementTypes, vector<craftMaterial>>(ARMOR, craftArmor));
}

void ElementFactory::update(int deltaTime)
{
	updateTime += deltaTime;
	if (updateTime >= MAXUPDATE_TIME)
	{
		updateTime -= MAXUPDATE_TIME;
		for (pair<glm::ivec2*, int> material : mapMaterials)
		{
			glm::ivec2 *position = material.first;
			position->y += 2;
			map->collisionMoveDown(*position, glm::ivec2(32, 32), &(position->y));
		}
	}
}

void ElementFactory::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void ElementFactory::addElement(int type)
{
	Element *element;
	switch (type)
	{
	case WEAPON:
		element = new Weapon();
		break;
	case PICK:
		element = new Pick();
		break;
	case ARMOR:
		element = new Armor();
		break;
	default:
		break;
	}
	addElement(element);
}

void ElementFactory::addElement(Element *element)
{
	bool found = false;
	unsigned int i = 0;
	while (i < elements.size() && found == false)
	{
		if (elements[i] == NULL) found = true;
		else i++;
	}

	if (found) elements[i] = element;
	else elements.push_back(element);
}

Element* ElementFactory::getElementByIndex(int index)
{
	return elements[index];
}

Element* ElementFactory::getElementSelected()
{
	return (elements[selected] != NULL) ? elements[selected] : NULL;
}

void ElementFactory::setElementIndexSelected(int selected)
{
	this->selected = selected;
}

int ElementFactory::getElementIndexSelected()
{
	return selected;
}

void ElementFactory::collectElement(int index)
{
	int type = mapMaterials[index].second;
	mapMaterials.erase(mapMaterials.begin() + index);

	bool found = false;
	int firstNull = -1;
	for (unsigned int i = 0; i < elements.size(); ++i){
		if (elements.at(i) == NULL && firstNull == -1) firstNull = i;
		if (dynamic_cast<Material*>(elements[i])) {
			Material *mat = dynamic_cast<Material*>(elements[i]);
			if (mat->getTileIndex() == type) {
				elements[i]->add(1);
				found = true;
			}
		}
	}

	if (!found)
	{
		if (firstNull != -1) elements[firstNull] = new Material(type);
		else elements.push_back(new Material(type));
	}
}

void ElementFactory::removeElement(Element *element)
{
	int pos = this->getElementPosition(element);
	if (pos != -1)
	{
		delete elements[pos];
		elements[pos] = NULL;
	}
}

void ElementFactory::consumeElement(Element *element, int quantity)
{
	element->consume(quantity);
	if (element->getQuantity() == 0)
		this->removeElement(element);
}

bool ElementFactory::equipElement(Element *element)
{
	if (element->canEquip()){
		equip.push_back(element->clone());
		removeElement(element);
		return true;
	}
	return false;
	
}

void ElementFactory::unequipElement(int index)
{
	addElement(equip[index]->clone());
	equip.erase(equip.begin() + index);
}

void ElementFactory::craftElement(int index)
{
	pair < int, vector<ElementFactory::craftMaterial >> itemToCraft = craftintMaterials.at(index);
	vector<int> indexMats;
	for (unsigned i = 0; i < elements.size(); ++i){
		if (elements.at(i) != NULL){
			if (elements.at(i)->getType() == MATERIAL)
				indexMats.push_back(i);
		}
	}
	for (unsigned int i = 0; i < itemToCraft.second.size(); ++i){
		craftMaterial material = itemToCraft.second.at(i);
		for (int k : indexMats)
		if (elements.at(k)->getTileIndex() == material.type)
			consumeElement(elements.at(k), material.quantity);
	}
	addElement(itemToCraft.first);
}

vector<pair<elementTypes, vector<ElementFactory::craftMaterial>>> ElementFactory::getCraftingMaterials()
{
	return craftintMaterials;
}

vector<pair<elementTypes, vector<ElementFactory::craftMaterial>>> ElementFactory::listCraftItems()
{
	itemsIndexToCraft.clear();
	vector<int> indexMats;
	vector < pair < elementTypes, vector<ElementFactory::craftMaterial >> > materialsCanCraft;
	for (unsigned i = 0; i < elements.size(); ++i){
		if (elements.at(i) != NULL){
			if (elements.at(i)->getType() == MATERIAL)
				indexMats.push_back(i);
		}
	}
	for (pair<elementTypes, vector<craftMaterial>> elementToCraft : craftintMaterials){
		bool found = true;
		unsigned int j = 0;
		while (j < elementToCraft.second.size() && found == true){
			craftMaterial material = elementToCraft.second.at(j);
			found = false;
			for (int k : indexMats){
				if (elements.at(k)->getTileIndex() == material.type)
					found = material.quantity <= elements.at(k)->getQuantity();
			}
			j++;
		}
		if (found) {
			materialsCanCraft.push_back(elementToCraft);
			itemsIndexToCraft.push_back(--j);
		}
	}

	return materialsCanCraft;
}

int ElementFactory::getElementPosition(Element *element)
{
	return find(elements.begin(), elements.end(), element) - elements.begin();
}

void ElementFactory::addMapMaterial(int type, glm::vec2 position)
{
	glm::ivec2 *pos = new glm::ivec2(position);
	mapMaterials.push_back(pair<glm::ivec2 *, int>(pos, type));
}

vector<pair<glm::ivec2*, int>> ElementFactory::getMapMaterials()
{
	return mapMaterials;
}
