#ifndef _ELEMENTMANAGER_INCLUDE
#define _ELEMENTMANAGER_INCLUDE

#include "ElementFactory.h"

class ElementManager
{
public:
	ElementManager(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram);


	void render();

	void setPosition(const glm::vec2 &minCoords);

	void setElementSelected(int selected);
	Element* getElementSelected();

	Element *addElement(int type);
	void removeElement(Element *element);
	bool craftElement(int type);
private:
	void prepareArrays();
private:
	TextureGenerator *textGenerator;
	TextureGenerator *textGeneratorSel;
	TextureGenerator *textGeneratorItems;

	ElementFactory *elementFactory;
};

#endif