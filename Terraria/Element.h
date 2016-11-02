#ifndef _ELEMENT_INCLUDE
#define _ELEMENT_INCLUDE

#include <vector>

enum elementTypes{
	PICK, WEAPON, MATERIAL, ARMOR
};

using namespace std;

class Element
{
public:
	float getDamage() const { return damage; }
	int getTileIndex() const { return tileIndex; }
	int getQuantity() const { return quantity; };
	bool canEquip() const { return equip; }

	void add(int quantity);
	void consume(int quantity);

	virtual Element* clone() const = 0;
	virtual elementTypes getType() = 0;
protected:
	float damage, attSpeed;
	int tileIndex, quantity;
	bool equip;
};

#endif // _ELEMENT_INCLUDE