#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <vector>

#define LEFT 0
#define RIGHT 1


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	struct tilesConfig{
		int hits;
		bool destroyable;
	};

	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	void prepareArrays(const glm::vec2 &minCoords, bool init = true);

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	
	bool playerSeenBy(const glm::vec2 &pos1, const glm::vec2 &pos2, int maxDistance);
	bool playerCollisionBy(const glm::vec2 &pos1, const glm::vec2 &pos2);

	bool insideDistance(const glm::vec2 &pos1, const glm::vec2 &pos2, int maxDistance);

	void buildElement(glm::ivec2 posElement, int type);
	int getElementType(glm::ivec2 posElement);
	int getElementHitsLeft(glm::ivec2 posElement);
	bool hitElement(glm::ivec2 posElement);
private:
	bool loadLevel(const string &levelFile);

private:
	GLuint vao;
	GLuint vbo;
	ShaderProgram *program;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;

	Brick **map;
	int nTiles;
	vector<float> vertices;
};


#endif // _TILE_MAP_INCLUDE


