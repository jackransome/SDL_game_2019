#include "WallCollection.h"

WallCollection::WallCollection() {}

WallCollection::~WallCollection() {}

void WallCollection::init(GameEngine::SpriteBatch* _sb) {
	sb = _sb;
}

void WallCollection::addWall(float _x, float _y, float _width, float _height) {
	wallVector.push_back(new Wall(_x, _y, _width, _height, sb));
}

void WallCollection::draw() {
	for (int i = 0; i < wallVector.size(); i++)
	{
		wallVector[i]->draw();
	}
}

void WallCollection::drawByIndex(int _index)
{
	if (_index < 0 || _index >= wallVector.size()) {
		fprintf(stderr, "wallVector _index out of range");
		return;
	}
	wallVector[_index]->draw();
}

void WallCollection::remove(int _index)
{
	wallVector.erase(wallVector.begin() + _index);
}

BoundingBox* WallCollection::getBoundingBox(int _index) {
	return wallVector[_index]->getBoundingBox();
}