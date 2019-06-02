#pragma once

#include <vector>
#include <string>

#include "WallTurret.h"

class WallTurretCollection {
public:
	WallTurretCollection();
	~WallTurretCollection();
	void init(GameEngine::SpriteBatch* _sb, ProjectileCollection* _projectiles);
	void addWallTurret(glm::vec2 _position, glm::vec2 _velocity);
	void launch(glm::vec2 _p1, glm::vec2 _p2, float _vel);
	void update();
	void setToStatic(int index);
	void draw();
	void changeHealth(int _index, int _amount);
	void remove(int _index);
	BoundingBox* getBoundingBox(int _index);
	int getVectorSize() { return wallTurretVector.size(); }
	void clearTargets();
	void target(int _index, BoundingBox* _boundingBox);
	glm::vec2 getPosition(int _index);
	glm::vec2 getCenter(int _index);
private:
	std::vector<WallTurret*> wallTurretVector;
	GameEngine::SpriteBatch* sb;
	ProjectileCollection* projectiles;
};
