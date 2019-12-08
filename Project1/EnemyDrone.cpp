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
	//mainSprite.init(sb, 8, 8, 2, 4, 8, 0, 0);
	//mainSprite.loadTexture("textures/enemyDroneMain.png");
	//shootingSprite.init(sb, 8, 8, 2, 2, 8, 0, 0);
	//shootingSprite.loadTexture("textures/enemyDroneShooting.png");
	mainSprite.init(sb, 8, 16, 2, 2, 8, 0, 0);
	mainSprite.loadTexture("textures/enemyDroneMain_2.png");
	shootingSprite.init(sb, 8, 16, 2, 2, 8, 0, 0);
	shootingSprite.loadTexture("textures/enemyDroneShooting_2.png");
	shootCoolDown = 0;
	shooting = false;
	shootRange = 200;
	sensorRange = 400;
	minRange = 100;
	speed = 5;
	health = 10;
	maxShootDown = 20;
	damagePower = 1;
}

void EnemyDrone::run()
{
	// |------|-----------|--------------|
	// Target |			  |              |
	//        Min range   |              |
	//                    Shooting range |
	// Shoots \/                         Sensor range
	// |------------------|
	// Moves closer to target \/
	//        |--------------------------|
	//                        Follows pathfinding \/
	//                                   |-------- - - - - - - - -   -   -   -   -   -

	// if drone has a target ( does not happen until drone is within sensor range )
	if (target) {
		if (pathSet) {

		}
		pathSet = false;
		float dist = sqrt(pow(target->x - boundingBox.x, 2) + pow(target->y - boundingBox.y, 2));
		if (dist < shootRange) {
			shooting = true;
		}
		else {
			shooting = false;
		}
		//if within sensor range but outside of min range: move towards target
		if (dist > minRange) {
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
		//if at min range: stop
		else if (dist <= minRange) {
			boundingBox.yv = boundingBox.xv = 0;
		}
		// if shooting (set to true above if within shooting range)
		if (shooting) {
			if (shootCoolDown == 0) {
				shootAt(*target);
				shootCoolDown = maxShootDown;
			}
			if (shootCoolDown > 0) {
				shootCoolDown--;
			}
		}
	}

	// If drone is not in line of sight of AND close enough to a target
	if (pathSet) {
		shooting = false;
		float dist = sqrt(pow((boundingBox.x + boundingBox.w / 2) - path->position.x, 2) + pow((boundingBox.y + boundingBox.h / 2) - path->position.y, 2));
		//if very close to first node in path: snap to it and remove first node from the list
		if (sqrt(pow((boundingBox.x + boundingBox.w / 2) - path->position.x, 2) + pow((boundingBox.y + boundingBox.h / 2) - path->position.y, 2)) <= speed) {
			boundingBox.x = path->position.x;
			boundingBox.y = path->position.y;
			Path* oldNode = path;
			path = path->next;
			//DELETE OLD PATH?
		}
		else {
			//if not close, move towards it
			float theta = atan(-1 * (boundingBox.y + boundingBox.h / 2 - path->position.y) / (boundingBox.x + boundingBox.w / 2 - path->position.x));
			if (boundingBox.x + boundingBox.w / 2 > path->position.x) {
				boundingBox.yv = -(sin(theta) * -1 * speed);
				boundingBox.xv = -(cos(theta) * speed);
			}
			else {
				boundingBox.yv = -(sin(theta) * speed);
				boundingBox.xv = -(cos(theta) * -1 * speed);
			}
		}
	}
}

void EnemyDrone::draw()
{
	if (shooting) {
		shootingSprite.run();
		shootingSprite.draw(boundingBox.x, boundingBox.y+16);
	}
	else {
		mainSprite.run();
		mainSprite.draw(boundingBox.x, boundingBox.y+16);
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

void EnemyDrone::setPath(Path* _path)
{
	path = _path;
	pathSet = true;
}

float EnemyDrone::getSensorRange()
{
	return sensorRange;
}

bool EnemyDrone::hasPath()
{
	return pathSet;
}

bool EnemyDrone::hasTarget()
{
	if (target) {
		return true;
	}
	return false;
}

void EnemyDrone::shootAt(BoundingBox _boundingBox)
{
	projectiles->launch(glm::vec2(boundingBox.x + boundingBox.w / 2, boundingBox.y + boundingBox.h / 2), glm::vec2(_boundingBox.x + _boundingBox.w / 2, _boundingBox.y + _boundingBox.h / 2), projectileSpeed, damageFriendly, damagePower);
}
