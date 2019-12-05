#include "WallTurretCollection.h"

WallTurretCollection::WallTurretCollection()
{
}

WallTurretCollection::~WallTurretCollection()
{
}

void WallTurretCollection::init(GameEngine::SpriteBatch * _sb, ProjectileCollection * _projectiles)
{
	sb = _sb;
	projectiles = _projectiles;
}

void WallTurretCollection::addWallTurret(glm::vec2 _position, glm::vec2 _velocity)
{
	wallTurretVector.push_back(new WallTurret(_position, _velocity, sb, projectiles));
}

void WallTurretCollection::launch(glm::vec2 _p1, glm::vec2 _p2, float _vel)
{

	float xVel;
	float yVel;
	float theta = atan(-1 * (_p1.y - _p2.y) / (_p1.x - _p2.x));
	if (_p1.x > _p2.x) {
		yVel = sin(theta) * -1 * _vel;
		xVel = cos(theta) * _vel;
	}
	else {
		yVel = sin(theta) * _vel;
		xVel = cos(theta) * -1 * _vel;
	}
	addWallTurret(_p1, glm::vec2(-xVel, -yVel));
}

void WallTurretCollection::update()
{
	for (int i = 0; i < wallTurretVector.size(); i++) {
		wallTurretVector[i]->calcNewPos();
		if (wallTurretVector[i]->getIsStatic()) {
			wallTurretVector[i]->run();
		}
		if (wallTurretVector[i]->getHealth() <= 0) {
			remove(i);
			i--;
		}
	}
}

void WallTurretCollection::remove(int _index)
{
	wallTurretVector.erase(wallTurretVector.begin() + _index);
}

void WallTurretCollection::setToStatic(int index)
{
	wallTurretVector[index]->setToStatic();
}

void WallTurretCollection::draw()
{
	for (int i = 0; i < wallTurretVector.size(); i++) {
		wallTurretVector[i]->draw();
	}
}

void WallTurretCollection::drawByIndex(int _index)
{
	if (_index < 0 || _index >= wallTurretVector.size()) {
		fprintf(stderr, "wallTurretVector _index out of range");
		return;
	}
	wallTurretVector[_index]->draw();
}

void WallTurretCollection::changeHealth(int _index, int _amount)
{
	wallTurretVector[_index]->changeHealth(_amount);
}

BoundingBox * WallTurretCollection::getBoundingBox(int _index)
{
	return wallTurretVector[_index]->getBoundingBox();
}

bool WallTurretCollection::getStatic(int _index)
{
	return wallTurretVector[_index]->getIsStatic();
}

float WallTurretCollection::getSensorRange(int _index)
{
	return wallTurretVector[_index]->getSensorRange();
}

void WallTurretCollection::clearTargets()
{
	for (int i = 0; i < wallTurretVector.size(); i++) {
		wallTurretVector[i]->setTarget(NULL);
	}
}

glm::vec2 WallTurretCollection::getPosition(int _index)
{
	BoundingBox temp = *wallTurretVector[_index]->getBoundingBox();
	return glm::vec2(temp.x, temp.y);
}

glm::vec2 WallTurretCollection::getCenter(int _index)
{
	BoundingBox temp = *wallTurretVector[_index]->getBoundingBox();
	return glm::vec2(temp.x + temp.w / 2, temp.y + temp.h / 2);
}

void WallTurretCollection::target(int _index, BoundingBox* _boundingBox)
{
	//if old target is null new target is closer that old target
	if (!wallTurretVector[_index]->getTarget() || sqrt(pow(_boundingBox->x - wallTurretVector[_index]->getBoundingBox()->x, 2) + pow(_boundingBox->y - wallTurretVector[_index]->getBoundingBox()->y, 2)) < sqrt(pow(wallTurretVector[_index]->getTarget()->x - wallTurretVector[_index]->getBoundingBox()->x, 2) + pow(wallTurretVector[_index]->getTarget()->y - wallTurretVector[_index]->getBoundingBox()->y, 2))) {
		//set this turrets target to the new one
		wallTurretVector[_index]->setTarget(_boundingBox);
	}
}
