#pragma once

#include <GameEngine\BoundingBox.h>
#include <GameEngine\SpriteBatch.h>
#include <GameEngine\ResourceManager.h>
#include <GameEngine\GLTexture.h>

class Wall {
public:
	Wall(float _x, float _y, float _width, float _height, GameEngine::SpriteBatch *_sb);
	~Wall();
	BoundingBox* getBoundingBox();
	void draw();
private:
	GameEngine::SpriteBatch *sb;
	BoundingBox boundingBox;
	GameEngine::GLTexture texture;
};