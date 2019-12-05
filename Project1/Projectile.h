#pragma once

#include <GameEngine\SpriteBatch.h>

enum DamageType { damageFriendly, damageEnemy };

class Projectile {
public:
	Projectile(GameEngine::SpriteBatch* _sb, float _x, float _y, float _xVel, float _yVel, DamageType _damageType, float _damageAmount);
	~Projectile();
	void calcNewPos();
	glm::vec2 getPosition() { return position; }
	glm::vec2 getLastPosition() { return lastPosition; }
	void run();
	void draw();
	DamageType getDamageType();
	bool toDelete;
	void setPosition(glm::vec2 _position);
	float getDamage();
private:
	int lifeSpan;
	int lifeCounter;
	DamageType damageType;
	float damage;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 lastPosition;
	glm::vec2 drawPosition;
	GameEngine::SpriteBatch* sb;
};