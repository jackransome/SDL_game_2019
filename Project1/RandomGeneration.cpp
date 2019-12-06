#include "RandomGeneration.h"
#include <ctime> 

void RandomGeneration::init(WallCollection * _walls, PathFinding * _pathFinding, GameEngine::CollisionDetection* _collisionDetection)
{
	walls = _walls;
	pathFinding = _pathFinding;
	collisionDetection = _collisionDetection;
	doorWidth = 50;
	wallThickness = 20;
	cellSize = 800;
	minRoomSize = 400;
	maxRoomSize = 750;
}

void RandomGeneration::generate()
{
	int roomSize;
	int gridSize = 5;

	//seeding rand()
	srand(time(NULL));
	bool leftDoor = true;
	bool rightDoor = true;
	bool topDoor = true;
	bool bottomDoor = true;
	//creating additional rooms
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {


			//getting a random size
			roomSize = std::rand() % (maxRoomSize - minRoomSize) + minRoomSize;
			//creating the room
			if (i == 0) {
				leftDoor = false;
			}
			else {
				leftDoor = true;
			}
			if (i == gridSize - 1) {
				rightDoor = false;
			}
			else {
				rightDoor = true;
			}
			if (j == 0) {
				bottomDoor = false;
			}
			else {
				bottomDoor = true;
			}
			if (j == gridSize - 1) {
				topDoor = false;
			}
			else {
				topDoor = true;
			}
			createRoom(i*cellSize, j*cellSize, roomSize, roomSize, leftDoor, rightDoor, topDoor, bottomDoor);
		}

	}

}

void RandomGeneration::createRoom(float _x, float _y, float _width, float _height, bool _leftDoor, bool _rightDoor, bool _topDoor, bool _bottomDoor)
{
	_x -= _width / 2;
	_y -= _width / 2;

	//bottom
	if (_bottomDoor) {
		walls->addWall(_x, _y, (_width - doorWidth) / 2, wallThickness);
		walls->addWall(_x + (_width + doorWidth) / 2, _y, (_width - doorWidth) / 2, wallThickness);
		//hallway
		walls->addWall(_x + (_width + doorWidth) / 2, _y - (cellSize - _height) / 2, wallThickness, (cellSize - _height) / 2);
		walls->addWall(_x + (_width - doorWidth) / 2 - wallThickness, _y - (cellSize - _height) / 2, wallThickness, (cellSize - _height) / 2);
	}
	else {
		walls->addWall(_x, _y, _width, wallThickness);
	}
	//top
	if (_topDoor) {
		walls->addWall(_x, _y + _height - wallThickness, (_width - doorWidth) / 2, wallThickness);
		walls->addWall(_x + (_width + doorWidth) / 2, _y + _height - wallThickness, (_width - doorWidth) / 2, wallThickness);
		//hallway
		walls->addWall(_x + (_width + doorWidth) / 2, _y + _height, wallThickness, (cellSize - _height) / 2);
		walls->addWall(_x + (_width - doorWidth) / 2 - wallThickness, _y + _height, wallThickness, (cellSize - _height) / 2);
	}
	else {
		walls->addWall(_x, _y + _height - wallThickness, _width, wallThickness);
	}
	//left
	if (_leftDoor) {
		walls->addWall(_x, _y, wallThickness, (_height - doorWidth)/2);
		walls->addWall(_x, _y + (_height + doorWidth)/2, wallThickness, (_height - doorWidth) / 2);
		//hallway
		walls->addWall(_x - (cellSize - _width) / 2, _y + (_height - doorWidth) / 2 - wallThickness, (cellSize - _width) / 2, wallThickness);
		walls->addWall(_x - (cellSize - _width) / 2, _y + (_height + doorWidth) / 2, (cellSize - _width) / 2, wallThickness);
	}
	else {
		walls->addWall(_x, _y, wallThickness, _height);
	}
	//right
	if (_rightDoor) {
		walls->addWall(_x + _width - wallThickness, _y, wallThickness, (_height - doorWidth) / 2);
		walls->addWall(_x + _width - wallThickness, _y + (_height + doorWidth) / 2, wallThickness, (_height - doorWidth) / 2);
		//hallway
		walls->addWall(_x + _width, _y + (_height - doorWidth) / 2 - wallThickness, (cellSize - _width) / 2, wallThickness);
		walls->addWall(_x + _width, _y + (_height + doorWidth) / 2, (cellSize - _width) / 2, wallThickness);
	}
	else {
		walls->addWall(_x + _width - wallThickness, _y, wallThickness, _height);
	}

	//adding nodes:
	pathFinding->addNode(glm::vec2(_x + _width / 2, _y + _height / 2));

}
