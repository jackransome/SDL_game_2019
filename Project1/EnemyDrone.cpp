#include "EnemyDrone.h"
#include <GameEngine/CollisionDetection.h>
EnemyDrone::EnemyDrone()
{
}

EnemyDrone::~EnemyDrone()
{
}

void EnemyDrone::init(glm::vec2 _position, GameEngine::SpriteBatch * _sb, ProjectileCollection* _projectiles)
{
	sb = _sb;
	projectiles = _projectiles;
	boundingBox.x = _position.x;
	boundingBox.y = _position.y;
	boundingBox.w = 16;
	boundingBox.h = 16;
	boundingBox.xv = 0;
	boundingBox.yv = 0;
	projectileSpeed = 6;
	mainSprite.init(sb, 8, 8, 2, 4, 8, 0, 0);
	mainSprite.loadTexture("textures/enemyDroneMain.png");
	shootingSprite.init(sb, 8, 8, 2, 2, 8, 0, 0);
	shootingSprite.loadTexture("textures/enemyDroneShooting.png");
	shootCoolDown = 0;
	shooting = false;
	shootRange = 200;
	sensorRange = 400;
	minRange = 100;
	speed = 2;
	health = 5;
}

void EnemyDrone::run()
{
	if (target) {
		float dist = sqrt(pow(target->x - boundingBox.x, 2) + pow(target->y - boundingBox.y, 2));
		if (dist < shootRange) {
			shooting = true;
		}
		else {
			shooting = false;
		}
		if (dist > minRange && dist < sensorRange) {
			float theta = atan(-1 * (boundingBox.y - target->y) / (boundingBox.x - target->x));
			if (boundingBox.x > target->x) {
				boundingBox.yv = -(sin(theta) * -1 * speed);
				boundingBox.xv = -(cos(theta) * speed);
			}
			else {
				boundingBox.yv = -(sin(theta) * speed);
				boundingBox.xv = -(cos(theta) * -1 * speed);
			}
		}
		else {
			boundingBox.yv = boundingBox.xv = 0;
		}
		if (shooting) {
			if (shootCoolDown == 0) {
				shootAt(*target);
				shootCoolDown = 20;
			}
			if (shootCoolDown > 0) {
				shootCoolDown--;
			}
		}
	}
	else {
		boundingBox.yv = boundingBox.xv = 0;
		shooting = false;
	}
}

void EnemyDrone::draw()
{
	if (shooting) {
		shootingSprite.run();
		shootingSprite.draw(boundingBox.x, boundingBox.y);
	}
	else {
		mainSprite.run();
		mainSprite.draw(boundingBox.x, boundingBox.y);
	}
}

void EnemyDrone::calcNewPos()
{
	boundingBox.x += boundingBox.xv;
	boundingBox.y += boundingBox.yv;
}

BoundingBox * EnemyDrone::getBoundingBox()
{
	return &boundingBox;
}

void EnemyDrone::setTarget(BoundingBox * _boundingBox)
{
	target = _boundingBox;
}

BoundingBox * EnemyDrone::getTarget()
{
	return target;
}

float EnemyDrone::getHealth()
{
	return health;
}

void EnemyDrone::changeHealth(float _amount)
{
	health += _amount;
}

void EnemyDrone::shootAt(BoundingBox _boundingBox)
{
	projectiles->launch(glm::vec2(boundingBox.x + boundingBox.w / 2, boundingBox.y + boundingBox.h / 2), glm::vec2(_boundingBox.x + _boundingBox.w / 2, _boundingBox.y + _boundingBox.h / 2), projectileSpeed, damageFriendly);
}
