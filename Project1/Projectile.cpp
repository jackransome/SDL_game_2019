
#include "Projectile.h"

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

void Projectile::init(GameEngine::SpriteBatch* _sb, float _x, float _y, float _xVel, float _yVel, DamageType _damageType, float _damage)
{
	position.x = _x;
	position.y = _y;
	velocity.x = _xVel;
	velocity.y = _yVel;
	lastPosition = position;
	drawPosition = lastPosition;
	sb = _sb;
	damageType = _damageType;
	lifeSpan = 100;
	lifeCounter = 0;
	toDelete = false;
	damage = _damage;
}

void Projectile::calcNewPos()
{
	if (!toDelete) {
		position += velocity;
	}
}
void Projectile::run()
{
	lifeCounter++;
	if (lifeCounter == lifeSpan) {
		toDelete = true;
	}
	drawPosition = lastPosition;
	lastPosition = position;
	calcNewPos();
}

void Projectile::draw()
{
	if (damageType == damageEnemy) {
		sb->drawLine(position, drawPosition, 60, 30, 220, 1, 1);
	}
	else if (damageType == damageFriendly) {
		sb->drawLine(position, drawPosition, 220, 30, 60, 1, 1);
	}

}

DamageType Projectile::getDamageType()
{
	return damageType;
}

void Projectile::setPosition(glm::vec2 _position)
{
	position = _position;
	lastPosition = _position - velocity;
	drawPosition = _position - glm::vec2(velocity.x * 2, velocity.y * 2);
}

float Projectile::getDamage()
{
	return damage;
}
