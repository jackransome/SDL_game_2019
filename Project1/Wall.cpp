#include "Wall.h"

Wall::Wall(float _x, float _y, float _width, float _height, GameEngine::SpriteBatch *_sb) {
	boundingBox.x = _x;
	boundingBox.y = _y;
	boundingBox.w = _width;
	boundingBox.h = _height;
	boundingBox.xv = 0;
	boundingBox.yv = 0;
	sb = _sb;
	texture = GameEngine::ResourceManager::getTexture("Textures/wall1.png");
}

Wall::~Wall() {}

BoundingBox* Wall::getBoundingBox() {
	return &boundingBox;
}

void Wall::draw() {
	GameEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	for (int i = 0; i < 15; i++) {
		sb->draw(glm::vec4(boundingBox.x, boundingBox.y+i, boundingBox.w, boundingBox.h), glm::vec4(0, 0, (boundingBox.w / 1000) / 2, (boundingBox.h / 1000) / 2), texture.id, 0.0f, color, 1);
	}
	
}