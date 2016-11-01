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
	Element();
	~Element();

	void render();
	
	float getDamage() const { return damage; }
	int getTileIndex() const { return tileIndex; }
	int getQuantity() const { return quantity; };
	
	void add(int quantity);
	void consume(int quantity);

	// TODO: struct with differents type of weapon/pick/material if there are more than one, and select type
	virtual int getType() = 0;
protected:
	float damage, attSpeed;
	int tileIndex, quantity;
};

#endif // _ELEMENT_INCLUDE