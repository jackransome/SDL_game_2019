
#include "Projectile.h"

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

void Projectile::init(GameEngine::SpriteBatch* _sb, float _x, float _y, float _xVel, float _yVel, DamageType _damageType)
{
	position.x = _x;
	position.y = _y;
	velocity.x = _xVel;
	velocity.y = _yVel;
	lastPosition = position;
	drawPosition = lastPosition;
	sb = _sb;
	damageType = _damageType;
}

void Projectile::calcNewPos()
{
	if (!toDelete) {
		position += velocity;
	}
}
void Projectile::run()
{
	drawPosition = lastPosition;
	lastPosition = position;
	calcNewPos();
}

void Projectile::draw()
{
	sb->drawLine(position, drawPosition, 255, 150, 0, 255, 1);
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
