#pragma once

#include <vector>
#include <string>

#include "Wall.h"

class WallCollection {
public:
	WallCollection();
	~WallCollection();
	void init(GameEngine::SpriteBatch* _sb);
	void addWall(float _x, float _y, float _width, float _height);
	void draw();
	void drawByIndex(int _index);
	void remove(int index);
	BoundingBox* getBoundingBox(int _index);
	int getVectorSize() { return wallVector.size(); }
private:
	std::vector<Wall*> wallVector;
	GameEngine::SpriteBatch* sb;
};
