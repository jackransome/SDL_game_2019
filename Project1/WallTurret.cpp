#include "WallTurret.h"

WallTurret::WallTurret()
{
}

WallTurret::~WallTurret()
{
}

void WallTurret::init(glm::vec2 _position, glm::vec2 _velocity, GameEngine::SpriteBatch * _sb, ProjectileCollection* _projectiles)
{
	sb = _sb;
	projectiles = _projectiles;
	boundingBox.x = _position.x;
	boundingBox.y = _position.y;
	boundingBox.w = 16;
	boundingBox.h = 16;
	boundingBox.xv = _velocity.x;
	boundingBox.yv = _velocity.y;
	projectileSpeed = 6;
	isStatic = false;
	movingSprite.init(sb, 8, 8, 2, 4, 8, 0, 0);
	movingSprite.loadTexture("textures/wallTurretMoving.png");
	staticSprite.init(sb, 8, 8, 2, 2, 32, 0, 0);
	staticSprite.loadTexture("textures/wallTurretStatic.png");
	shootCoolDown = 0;
}

void WallTurret::run()
{
	if (shootCoolDown == 0) {
		shootAt(*target);
		shootCoolDown = 20;
	}
	if (shootCoolDown > 0) {
		shootCoolDown--;
	}
	
}

void WallTurret::draw()
{
	if (isStatic) {
		staticSprite.run();
		staticSprite.draw(boundingBox.x, boundingBox.y);
	}
	else {
		movingSprite.run();
		movingSprite.draw(boundingBox.x, boundingBox.y);
	}
}

void WallTurret::calcNewPos()
{
	boundingBox.x += boundingBox.xv;
	boundingBox.y += boundingBox.yv;
}

bool WallTurret::getIsStatic()
{
	return isStatic;
}

void WallTurret::setToStatic()
{
	isStatic = true;
	boundingBox.xv = boundingBox.yv = 0;
}

BoundingBox * WallTurret::getBoundingBox()
{
	return &boundingBox;
}

void WallTurret::setTarget(BoundingBox * _boundingBox)
{
	target = _boundingBox;
}

void WallTurret::shootAt(BoundingBox _boundingBox)
{
	projectiles->launch(glm::vec2(boundingBox.x, boundingBox.y), glm::vec2(_boundingBox.x  + _boundingBox.w / 2, _boundingBox.y + _boundingBox.h / 2), projectileSpeed, damageEnemy);
}
