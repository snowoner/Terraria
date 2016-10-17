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
private:
	vector<Element*> craftingMaterials;
};

#endif // _ELEMENT_INCLUDE