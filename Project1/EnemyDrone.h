#pragma once

#include "GameEngine/SpriteSheet.h"
#include "GameEngine/SpriteBatch.h"
#include <GameEngine/BoundingBox.h>
#include <GameEngine/GameEngine.h>

#include "ProjectileCollection.h"

class EnemyDrone {
public:

	EnemyDrone();
	~EnemyDrone();
	void init(glm::vec2 _position, GameEngine::SpriteBatch* _sb, ProjectileCollection* _projectiles);
	void run();
	void draw();
	void calcNewPos();
	BoundingBox* getBoundingBox();
	void setTarget(BoundingBox* _boundingBox);
	BoundingBox* getTarget();
	float getHealth();
	void changeHealth(float _amount);

private:

	int shootCoolDown;
	void shootAt(BoundingBox _boundingBox);
	BoundingBox* target;
	float projectileSpeed;
	ProjectileCollection* projectiles;
	GameEngine::SpriteBatch* sb;
	BoundingBox boundingBox;
	bool shooting;
	int sensorRange;
	int shootRange;
	int minRange;
	float health;
	float speed;

	GameEngine::SpriteSheet mainSprite;
	GameEngine::SpriteSheet shootingSprite;
};