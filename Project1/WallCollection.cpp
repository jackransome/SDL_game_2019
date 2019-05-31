#include "WallCollection.h"

WallCollection::WallCollection() {}
WallCollection::~WallCollection() {}
void WallCollection::init(GameEngine::SpriteBatch* _sb) {
	sb = _sb;
}
void WallCollection::addWall(float _x, float _y, float _width, float _height) {
	wallVector.push_back(new Wall);
	wallVector[wallVector.size() - 1]->init(_x, _y, _width, _height);
}
void WallCollection::draw() {
	for (int i = 0; i < wallVector.size(); i++)
	{
		wallVector[i]->draw(sb);
	}
}
BoundingBox* WallCollection::getBoundingBox(int _index) {
	return wallVector[_index]->getBoundingBox();
}