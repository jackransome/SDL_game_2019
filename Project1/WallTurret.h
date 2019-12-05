#pragma once

#include "GameEngine/SpriteSheet.h"
#include "GameEngine/SpriteBatch.h"
#include <GameEngine/BoundingBox.h>
#include <GameEngine/GameEngine.h>

#include "ProjectileCollection.h"

class WallTurret {
public:

	WallTurret(glm::vec2 _position, glm::vec2 _velocity, GameEngine::SpriteBatch * _sb, ProjectileCollection* _projectiles);
	~WallTurret();
	void run();
	void draw();
	void calcNewPos();
	bool getIsStatic();
	void setToStatic();
	float getHealth();
	void changeHealth(float _amount);
	float getSensorRange();
	BoundingBox* getBoundingBox();
	BoundingBox* getTarget();
	void setTarget(BoundingBox* _boundingBox);

private:
	float damagePower;
	int sensorRange;
	int maxShootDown;
	int shootCoolDown;
	void shootAt(BoundingBox _boundingBox);
	BoundingBox* target;
	float projectileSpeed;
	ProjectileCollection* projectiles;
	GameEngine::SpriteBatch* sb;
	BoundingBox boundingBox;
	bool isStatic;
	float health;

	GameEngine::SpriteSheet movingSprite;
	GameEngine::SpriteSheet staticSprite;
	GameEngine::SpriteSheet shootingSprite;
};