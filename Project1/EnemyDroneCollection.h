#pragma once

#include <vector>
#include <string>
#include <GameEngine/Camera2D.h>

#include "EnemyDrone.h"


class EnemyDroneCollection {
public:
	EnemyDroneCollection();
	~EnemyDroneCollection();
	void init(GameEngine::SpriteBatch* _sb, ProjectileCollection* _projectiles, GameEngine::Camera2D *_cameraPointer);
	void addEnemyDrone(glm::vec2 _position);
	void run();
	void draw();
	BoundingBox* getBoundingBox(int _index);
	void drawByIndex(int _index);
	void changeHealth(int _index, int _amount);
	void remove(int _index);
	int getVectorSize() { return enemyDroneVector.size(); }
	void target(int _index, BoundingBox* _boundingBox);
	void clearTargets();
	glm::vec2 getPosition(int _index);
	glm::vec2 getCenter(int _index);
	bool hasTarget(int _index);
	void setPath(int _index, Path* _path);
	bool hasPath(int _index);
	float getSensorRange(int _index);
	Path* getPath(int _index);
private:
	GameEngine::Camera2D *cameraPointer;
	std::vector<EnemyDrone*> enemyDroneVector;
	GameEngine::SpriteBatch* sb;
	ProjectileCollection* projectiles;
};
