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
	wallTurretVector.push_back(new WallTurret);
	wallTurretVector[wallTurretVector.size() - 1]->init(_position, _velocity, sb, projectiles);
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
	}
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

BoundingBox * WallTurretCollection::getBoundingBox(int _index)
{
	return wallTurretVector[_index]->getBoundingBox();
}

void WallTurretCollection::target(BoundingBox* _boundingBox)
{
	for (int i = 0; i < wallTurretVector.size(); i++) {
		wallTurretVector[i]->setTarget(_boundingBox);
	}
}
