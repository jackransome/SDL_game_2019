#pragma once
#include "WallCollection.h"
#include "PathFinding.h"
#include <GameEngine/CollisionDetection.h>
class RandomGeneration {
public:
	void init(WallCollection* _walls, PathFinding* _pathFinding, GameEngine::CollisionDetection* _collisionDetection);
	void generate();
private:
	void createRoom(float _x, float _y, float _width, float _height, bool _leftDoor, bool _rightDoor, bool _topDoor, bool _bottomDoor);

	float doorWidth;
	float wallThickness;
	int cellSize;

	WallCollection* walls;
	GameEngine::CollisionDetection* collisionDetection;
	PathFinding* pathFinding;

	std::vector<BoundingBox> rooms;
};