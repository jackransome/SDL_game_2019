#pragma once
#include <vector>
#include <glm.hpp>
#include <GameEngine/SpriteBatch.h>
#include <GameEngine/CollisionDetection.h>

#include "Path.h"

struct Node {
	int numberOfNeighbors = 0;
	// used only in setting up the nodes
	std::vector<Node*> neighborsVector;

	Node **neighbors;
	int *distances;
	glm::vec2 position;
	// used in pathfinding algorithm to find path
	Node* previous = NULL;
	// used in pathfinding algorithm to avoid going backward / in loops
	bool visited;

	Path *cachedPath = NULL;
};

class PathFinding {
public:
	void init(GameEngine::SpriteBatch* _sb);
	void clearNodeData();
	void addNode(glm::vec2 _position);
	void connectNodes(int _index1, int _index2);
	void disconnectNodes(int _index1, int _index2);
	void optimiseNetwork();
	bool isConnectedTo(int _index1, int _index2);
	void draw();
	void fillNeighbors();
	Path* getPath(int _index1, int _index2);
	int getVectorSize() { return nodes.size(); }
	glm::vec2 getPosition(int _index);
private:
	GameEngine::SpriteBatch* sb;
	std::vector<Node*> nodes;
	std::vector<std::vector<bool>> adjacent;
};