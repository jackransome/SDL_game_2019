#pragma once

#include <GameEngine\BoundingBox.h>
#include <GameEngine\InputManager.h>
#include <GameEngine\SpriteBatch.h>
#include <GameEngine\SpriteSheet.h>
#include <GameEngine\Camera2D.h>

class Player
{
public:
	Player();
	~Player();
	void init(float _x, float _y, GameEngine::SpriteBatch* _sb, GameEngine::Camera2D* _c);
	BoundingBox* getBoundingBox();
	void handleInput(GameEngine::InputManager* _im);
	void calcNewPos();
	float getHealth();
	void changeHealth(float _amount);
	glm::vec2 getCenter();
	void draw();
private:
	GameEngine::Camera2D* camera;
	GameEngine::SpriteBatch* sb;
	BoundingBox boundingBox;

	GameEngine::SpriteSheet mainSprite;

	float speed;
	float health;

};

