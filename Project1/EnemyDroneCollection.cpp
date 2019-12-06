#include "EnemyDroneCollection.h"
EnemyDroneCollection::EnemyDroneCollection() {}

EnemyDroneCollection::~EnemyDroneCollection() {}

void EnemyDroneCollection::init(GameEngine::SpriteBatch * _sb, ProjectileCollection * _projectiles, GameEngine::Camera2D *_cameraPointer)
{
	sb = _sb;
	projectiles = _projectiles;
	cameraPointer = _cameraPointer;
}

void EnemyDroneCollection::addEnemyDrone(glm::vec2 _position)
{
	enemyDroneVector.push_back(new EnemyDrone);
	enemyDroneVector[enemyDroneVector.size() - 1]->init(_position, sb, projectiles);
}

void EnemyDroneCollection::update()
{
	for (int i = 0; i < enemyDroneVector.size(); i++) {
		enemyDroneVector[i]->calcNewPos();
		enemyDroneVector[i]->run();
		if (enemyDroneVector[i]->getHealth() <= 0) {
			cameraPointer->setScreenShakeIntensity(5);
			remove(i);
			i--;
		}
	}
}

void EnemyDroneCollection::draw()
{
	for (int i = 0; i < enemyDroneVector.size(); i++) {
		enemyDroneVector[i]->draw();
	}
}

void EnemyDroneCollection::drawByIndex(int _index)
{
	if (_index < 0 || _index >= enemyDroneVector.size()) {
		fprintf(stderr, "enemyDroneVector _index out of range");
		return;
	}
	enemyDroneVector[_index]->draw();
}

BoundingBox * EnemyDroneCollection::getBoundingBox(int _index)
{
	return enemyDroneVector[_index]->getBoundingBox();
}

void EnemyDroneCollection::remove(int _index)
{
	enemyDroneVector.erase(enemyDroneVector.begin() + _index);
}

void EnemyDroneCollection::changeHealth(int _index, int _amount)
{
	enemyDroneVector[_index]->changeHealth(_amount);
}

void EnemyDroneCollection::clearTargets()
{
	for (int i = 0; i < enemyDroneVector.size(); i++) {
		enemyDroneVector[i]->setTarget(NULL);
	}
}

glm::vec2 EnemyDroneCollection::getPosition(int _index)
{
	BoundingBox temp = *enemyDroneVector[_index]->getBoundingBox();
	return glm::vec2(temp.x, temp.y);
}

glm::vec2 EnemyDroneCollection::getCenter(int _index)
{
	BoundingBox temp = *enemyDroneVector[_index]->getBoundingBox();
	return glm::vec2(temp.x + temp.w / 2, temp.y + temp.h / 2);
}

bool EnemyDroneCollection::hasTarget(int _index)
{
	return enemyDroneVector[_index]->hasTarget();
}

void EnemyDroneCollection::setPath(int _index, Path* _path)
{
	enemyDroneVector[_index]->setPath(_path);
}

bool EnemyDroneCollection::hasPath(int _index)
{
	return enemyDroneVector[_index]->hasPath();
}

float EnemyDroneCollection::getSensorRange(int _index)
{
	return enemyDroneVector[_index]->getSensorRange();
}

void EnemyDroneCollection::target(int _index, BoundingBox* _boundingBox)
{
	//if old target is null or new target is closer that old target
	if (!enemyDroneVector[_index]->getTarget() || sqrt(pow(_boundingBox->x - enemyDroneVector[_index]->getBoundingBox()->x, 2) + pow(_boundingBox->y - enemyDroneVector[_index]->getBoundingBox()->y, 2)) < sqrt(pow(enemyDroneVector[_index]->getTarget()->x - enemyDroneVector[_index]->getBoundingBox()->x, 2) + pow(enemyDroneVector[_index]->getTarget()->y - enemyDroneVector[_index]->getBoundingBox()->y, 2))) {
		//set this turrets target to the new one
		enemyDroneVector[_index]->setTarget(_boundingBox);
	}
}
