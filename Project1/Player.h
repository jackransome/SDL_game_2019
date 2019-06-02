#pragma once

#include <GameEngine\BoundingBox.h>
#include <GameEngine\InputManager.h>
#include <GameEngine\SpriteBatch.h>
#include <GameEngine\SpriteSheet.h>
#include <GameEngine\Camera2D.h>

#include "ProjectileCollection.h"
#include "WallTurretCollection.h"
#include "EnemyDroneCollection.h"

class Player
{
public:
	Player();
	~Player();
	void init(float _x, float _y, GameEngine::SpriteBatch* _sb, GameEngine::Camera2D* _c, ProjectileCollection* _projectiles, WallTurretCollection* _wallTurrets);
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

	ProjectileCollection* projectiles;

	WallTurretCollection* wallTurrets;

	GameEngine::SpriteSheet mainSprite;

	float speed;
	float health;

};

