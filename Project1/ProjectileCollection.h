#pragma once
#include <vector>

#include <GameEngine\CollisionDetection.h>

#include "Projectile.h"

class ProjectileCollection {
public:
	ProjectileCollection();
	~ProjectileCollection();
	void init(GameEngine::SpriteBatch* _sb);
	void add(float _x, float _y, float _xVel, float _yVel, DamageType _damageType);
	void draw();
	void run();
	int getVectorSize() { return projectileVector.size(); }
	Projectile* getProjectile(int _index) { return projectileVector[_index]; }
	float getDamage(int _index);
	DamageType getDamageType(int _index);
	void remove(int _index);
	void launch(glm::vec2 _p1, glm::vec2 _p2, float _vel, DamageType _damageType);
	void setToDelete(int _index, bool _bool);
	bool getToDelete(int _index);
private:
	std::vector<Projectile*> projectileVector;
	GameEngine::SpriteBatch* sb;
	GameEngine::CollisionDetection cd;
};
