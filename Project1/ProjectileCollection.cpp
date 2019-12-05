
#include "ProjectileCollection.h"

ProjectileCollection::ProjectileCollection() {}

ProjectileCollection::~ProjectileCollection(){}

void ProjectileCollection::init(GameEngine::SpriteBatch * _sb){
	sb = _sb;
}

void ProjectileCollection::add(float _x, float _y, float _xVel, float _yVel, DamageType _damageType, float _damageAmount)
{
	projectileVector.push_back(new Projectile(sb, _x, _y, _xVel, _yVel, _damageType, _damageAmount));
}

void ProjectileCollection::draw()
{
	for (int i = 0; i < projectileVector.size(); i++) {
		projectileVector[i]->draw();
	}
}

void ProjectileCollection::drawByIndex(int _index)
{
	if (_index < 0 || _index >= projectileVector.size()) {
		fprintf(stderr, "projectileVector _index out of range");
		return;
	}
	projectileVector[_index]->draw();
}

glm::vec2 ProjectileCollection::getPosition(int _index)
{
	return projectileVector[_index]->getPosition();
}

void ProjectileCollection::run()
{
	for (int i = 0; i < projectileVector.size(); i++) {
		projectileVector[i]->run();
		if (projectileVector[i]->toDelete) {
			remove(i);
			i--;
		}
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

void ProjectileCollection::launch(glm::vec2 _p1, glm::vec2 _p2, float _vel, DamageType _damageType, float _damageAmount)
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
	add(_p1.x, _p1.y, -xVel, -yVel, _damageType, _damageAmount);
}

void ProjectileCollection::setToDelete(int _index, bool _bool)
{
	projectileVector[_index]->toDelete = _bool;
}

bool ProjectileCollection::getToDelete(int _index)
{
	return projectileVector[_index]->toDelete;
}
