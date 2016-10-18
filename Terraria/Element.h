#ifndef _ELEMENT_INCLUDE
#define _ELEMENT_INCLUDE

#include <vector>
using namespace std;

class Element
{
public:
	Element();
	~Element();

	void render();

	vector<Element*> getCraftingMaterials();

	float getDamage();

	// TODO: struct with differents type of weapon/pick/material if there are more than one, and select type
	virtual int getType() = 0;
protected:
	float damage, attSpeed;
	vector<Element*> craftingMaterials;
};

#endif // _ELEMENT_INCLUDE