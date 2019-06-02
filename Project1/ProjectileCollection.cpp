
#include "ProjectileCollection.h"

ProjectileCollection::ProjectileCollection()
{
}

ProjectileCollection::~ProjectileCollection()
{
}

void ProjectileCollection::init(GameEngine::SpriteBatch * _sb)
{
	sb = _sb;
}

void ProjectileCollection::add(float _x, float _y, float _xVel, float _yVel, DamageType _damageType)
{
	projectileVector.push_back(new Projectile());
	projectileVector[projectileVector.size() - 1]->init(sb, _x, _y, _xVel, _yVel, _damageType, 1);

}

void ProjectileCollection::draw()
{
	for (int i = 0; i < projectileVector.size(); i++) {
		projectileVector[i]->draw();
	}
}

void ProjectileCollection::run()
{
	for (int i = 0; i < projectileVector.size(); i++) {
		projectileVector[i]->run();
	}
}

float ProjectileCollection::getDamage(int _index)
{
	return projectileVector[_index]->getDamage();
}

DamageType ProjectileCollection::getDamageType(int _index)
{
	return projectileVector[_index]->getDamageType();
}

void ProjectileCollection::remove(int _index)
{
	projectileVector.erase(projectileVector.begin() + _index);
}

void ProjectileCollection::launch(glm::vec2 _p1, glm::vec2 _p2, float _vel, DamageType _damageType)
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
	add(_p1.x, _p1.y, -xVel, -yVel, _damageType);
}

void ProjectileCollection::setToDelete(int _index, bool _bool)
{
	projectileVector[_index]->toDelete = _bool;
}

bool ProjectileCollection::getToDelete(int _index)
{
	return projectileVector[_index]->toDelete;
}
