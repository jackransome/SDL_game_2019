#pragma once
#include <vector>
#include <glm.hpp>
#include <GameEngine/SpriteBatch.h>
struct Node {
	int numberOfNeighbors;
	std::vector<Node*> neighbors;
	Node* previous = NULL;
	glm::vec2 position;
	bool visited;
};

class PathFinding {
public:
	void init(GameEngine::SpriteBatch* _sb);
	void addNode(glm::vec2 _position);
	void connectNodes(int _index1, int _index2);
	void draw();
	std::vector<glm::vec2> getPath(int _index1, int _index2);
private:
	GameEngine::SpriteBatch* sb;
	std::vector<Node*> nodes;
	std::vector<std::vector<bool>> adjacent;
};