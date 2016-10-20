#ifndef _ELEMENTFACTORY_INCLUDE
#define _ELEMENTFACTORY_INCLUDE

#include "TextureGenerator.h"
#include "Element.h"
#include "Sprite.h"
#include "Weapon.h"
#include "Pick.h"
#include "Material.h"


class ElementFactory
{
public:
	ElementFactory(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram);

	void render();

	void setPosition(const glm::vec2 &minCoords);

	void setElementSelected(int selected);
	Element* getElementSelected();

	Element *addElement(int type);
	void removeElement(Element *element);
	bool craftElement(int type);
private:
	int getElementPosition(Element *element);
	void prepareArrays();
private:
	TextureGenerator *textGenerator;
	TextureGenerator *textGeneratorItems;

	vector<Element*> elements;
	int selected;
};

#endif // _ELEMENTFACTORY_INCLUDE