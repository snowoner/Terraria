#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	this->program = &program;
	prepareArrays(minCoords);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	vertices.clear();
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];



	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j*mapSize.x + i] = 0;
			else
				map[j*mapSize.x + i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, bool init)
{
	nTiles = 0;
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	int n = 0;
	if (init)
	{
		for (int j = 0; j < mapSize.y; j++)
		{
			for (int i = 0; i < mapSize.x; i++)
			{
				tile = map[j * mapSize.x + i];
				if (tile != 0)
				{
					// Non-empty tile
					nTiles++;
					posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
					texCoordTile[0] = glm::vec2(float((tile - 1) % 2) / tilesheetSize.x, float((tile - 1) / 2) / tilesheetSize.y);
					texCoordTile[1] = texCoordTile[0] + tileTexSize;
					texCoordTile[1] -= halfTexel;
					// First triangle
					vertices.push_back(posTile.x); vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
					vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
					vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
					vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
					// Second triangle
					vertices.push_back(posTile.x); vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
					vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
					vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
					vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
					vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
				}
			}
		}
	}
	else {
		for (int j = 0; j < mapSize.y; j++)
		{
			for (int i = 0; i < mapSize.x; i++)
			{
				tile = map[j * mapSize.x + i];
				if (tile != 0)
				{
					nTiles++;
					posTile = glm::vec2(minCoords.x + i*tileSize, minCoords.y + j *tileSize);
					texCoordTile[0] = glm::vec2(float((tile - 1) % 2) / tilesheetSize.x, float((tile - 1) / 2) / tilesheetSize.y);
					texCoordTile[1] = texCoordTile[0] + tileTexSize;
					texCoordTile[1] -= halfTexel;
					// First triangle
					vertices[n] = posTile.x; vertices[n + 1] = posTile.y;
					vertices[n + 2] = texCoordTile[0].x; vertices[n + 3] = texCoordTile[0].y;
					vertices[n + 4] = posTile.x + blockSize; vertices[n + 5] = posTile.y;
					vertices[n + 6] = texCoordTile[1].x; vertices[n + 7] = texCoordTile[0].y;
					vertices[n + 8] = posTile.x + blockSize; vertices[n + 9] = posTile.y + blockSize;
					vertices[n + 10] = texCoordTile[1].x; vertices[n + 11] = texCoordTile[1].y;
					// Second triangle
					vertices[n + 12] = posTile.x; vertices[n + 13] = posTile.y;
					vertices[n + 14] = texCoordTile[0].x; vertices[n + 15] = texCoordTile[0].y;
					vertices[n + 16] = posTile.x + blockSize; vertices[n + 17] = posTile.y + blockSize;
					vertices[n + 18] = texCoordTile[1].x; vertices[n + 19] = texCoordTile[1].y;
					vertices[n + 20] = posTile.x; vertices[n + 21] = posTile.y + blockSize;
					vertices[n + 22] = texCoordTile[0].x; vertices[n + 23] = texCoordTile[1].y;
					n += 24;
				}
			}
		}
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

// Build Material
void TileMap::buildElement(glm::ivec2 posElement, int type)
{
	// Do it to TileMap (getting material with player.removeElement()) and building it in tileMap
	map[posElement.y*mapSize.x + posElement.x] = type;
	vertices.resize(vertices.size() + ((type == 0) ? -1 : 1) * 24);

}

// Get Material
int TileMap::getElement(glm::ivec2 posElement)
{
	// Do it to TileMap (getting material with player.removeElement()) and building it in tileMap
	return map[posElement.y*mapSize.x + posElement.x];
}


// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y*mapSize.x + x] != 0)
			return true;
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y*mapSize.x + x] != 0)
			return true;
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}

	return false;
}

// TODO: get pos1.tileSize and pos2.tilesize
bool TileMap::playerSeenBy(const glm::vec2 &pos1, const glm::vec2 &pos2, int maxDistance)
{
	float x1 = (pos2.x > pos1.x) ? (pos1.x + 32 - 1) / tileSize : pos1.x / tileSize;
	float x2 = pos2.x / tileSize;
	float y1 = (pos2.y < pos1.y) ? (pos1.y + 32 - 1) / tileSize : pos1.y / tileSize;
	float y2 = pos2.y / tileSize;

	bool seen = true;

	// Bresenham's line algorithm
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;
	int x = (int)x1;
	while (x < maxX && x < maxDistance + (int)x1)
	{
		if (steep)
		{
			if (map[x*mapSize.x + y] != 0)
				return false;
		}
		else
		{
			if (map[y*mapSize.x + x] != 0)
				return false;
		}

		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
		x++;
	}
	return seen && x != (maxDistance + (int)x1);
}

bool TileMap::playerCollisionBy(const glm::vec2 &pos1, const glm::vec2 &pos2)
{
	// TODO: Select size from pos1 obj, and pos2 obj
	float a = 32.f / tileSize;
	float b = 32.f / tileSize;
	float x1 = pos1.x / tileSize;
	float x2 = pos2.x / tileSize;
	float y1 = pos1.y / tileSize;
	float y2 = pos2.y / tileSize;

	return ((x1 < x2 + a) &&
		(x2 < x1 + a) &&
		(y1 < y2 + b) &&
		(y2 < y1 + a)
		);
}

// TODO: get pos1.tileSize and pos2.tilesize
bool TileMap::insideDistance(const glm::vec2 &pos1, const glm::vec2 &pos2, int maxDistance) {
	float x1 = (pos2.x > pos1.x) ? (pos1.x + 32 - 1) / tileSize : pos1.x / tileSize;
	float x2 = pos2.x / tileSize;
	float y1 = (pos2.y < pos1.y) ? (pos1.y + 32 - 1) / tileSize : pos1.y / tileSize;
	float y2 = pos2.y / tileSize;

	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	return (std::round(x2 - x1) <= maxDistance);
}