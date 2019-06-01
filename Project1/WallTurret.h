#pragma once

#include "GameEngine/SpriteSheet.h"
#include "GameEngine/SpriteBatch.h"
#include <GameEngine/BoundingBox.h>
#include <GameEngine/GameEngine.h>

#include "ProjectileCollection.h"

class WallTurret {
public:

	WallTurret();
	~WallTurret();
	void init(glm::vec2 _position, glm::vec2 _velocity, GameEngine::SpriteBatch* _sb, ProjectileCollection* _projectiles);
	void run();
	void draw();
	void calcNewPos();
	bool getIsStatic();
	void setToStatic();
	BoundingBox* getBoundingBox();
	void setTarget(BoundingBox* _boundingBox);

private:
	
	int shootCoolDown;
	void shootAt(BoundingBox _boundingBox);
	BoundingBox* target;
	float projectileSpeed;
	ProjectileCollection* projectiles;
	GameEngine::SpriteBatch* sb;
	BoundingBox boundingBox;
	bool isStatic;

	GameEngine::SpriteSheet movingSprite;
	GameEngine::SpriteSheet staticSprite;
};