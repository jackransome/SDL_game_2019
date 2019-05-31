#pragma once

#include <GameEngine\BoundingBox.h>
#include <GameEngine\SpriteBatch.h>
#include <GameEngine\ResourceManager.h>
#include <GameEngine\GLTexture.h>

class Wall {
public:
	Wall();
	~Wall();
	void init(float _x, float _y, float _width, float _height);
	BoundingBox* getBoundingBox();
	void draw(GameEngine::SpriteBatch* sb);
private:
	BoundingBox boundingBox;
	GameEngine::GLTexture texture;
};