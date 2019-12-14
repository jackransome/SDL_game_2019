#include "WallTurret.h"

WallTurret::WallTurret(glm::vec2 _position, glm::vec2 _velocity, GameEngine::SpriteBatch * _sb, ProjectileCollection* _projectiles){
	sb = _sb;
	projectiles = _projectiles;
	boundingBox.x = _position.x;
	boundingBox.y = _position.y;
	boundingBox.w = 16;
	boundingBox.h = 16;
	boundingBox.xv = _velocity.x;
	boundingBox.yv = _velocity.y;
	projectileSpeed = 20;
	isStatic = false;
	movingSprite.init(sb, 8, 16, 2, 1, 8, 0, 0);
	movingSprite.loadTexture("textures/wallTurretMoving.png");
	staticSprite.init(sb, 8, 16, 2, 2, 32, 0, 0);
	staticSprite.loadTexture("textures/wallTurretStatic_2.png");
	shootingSprite.init(sb, 8, 16, 2, 2, 8, 0, 0);
	shootingSprite.loadTexture("textures/wallTurretShooting.png");
	shootCoolDown = 0;
	maxShootDown = 20;
	sensorRange = 400;
	health = 20;
	damagePower = 1;
}

WallTurret::~WallTurret() {}

void WallTurret::run()
{
	if (shootCoolDown == 0 && target && sqrt(pow(target->x - boundingBox.x, 2) + pow(target->y - boundingBox.y, 2)) < sensorRange) {
		shootAt(*target);
		shootCoolDown = maxShootDown;
	}
	if (shootCoolDown > 0) {
		shootCoolDown--;
	}
}

void WallTurret::draw()
{
	if (isStatic) {
		if (target) {
			shootingSprite.run();
			for (int i = 0; i < 15; i++) {
				shootingSprite.draw(boundingBox.x, boundingBox.y + 1);
			}
			//shootingSprite.draw(boundingBox.x, boundingBox.y);
		}
		else {
			staticSprite.run();
			staticSprite.draw(boundingBox.x, boundingBox.y);
		}
	}
	else {
		movingSprite.run();
		for (int i = 0; i < 15; i++) {
			movingSprite.draw(boundingBox.x, boundingBox.y + i);
		}
		//movingSprite.draw(boundingBox.x, boundingBox.y);
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

float WallTurret::getHealth()
{
	return health;
}

void WallTurret::changeHealth(float _amount)
{
	health += _amount;
}

float WallTurret::getSensorRange()
{
	return sensorRange;
}

BoundingBox * WallTurret::getBoundingBox()
{
	return &boundingBox;
}

BoundingBox * WallTurret::getTarget()
{
	if (!target) {
		return NULL;
	}
	return target;
}

void WallTurret::setTarget(BoundingBox * _boundingBox)
{
	target = _boundingBox;
}

void WallTurret::shootAt(BoundingBox _boundingBox)
{
	projectiles->launch(glm::vec2(boundingBox.x + boundingBox.w / 2, boundingBox.y + boundingBox.h / 2), glm::vec2(_boundingBox.x + _boundingBox.w / 2, _boundingBox.y + _boundingBox.h / 2), projectileSpeed, damageEnemy, damagePower);
}
