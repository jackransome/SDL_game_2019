#include "PathFinding.h"

void PathFinding::init(GameEngine::SpriteBatch * _sb)
{
	sb = _sb;
}

void PathFinding::addNode(glm::vec2 _position)
{
	nodes.push_back(new Node);
	nodes[nodes.size() - 1]->position = _position;
	
}

void PathFinding::connectNodes(int _index1, int _index2)
{
	nodes[_index1]->neighbors.push_back(nodes[_index2]);
	nodes[_index2]->neighbors.push_back(nodes[_index1]);
}

void PathFinding::draw()
{
	GameEngine::Color nodeColor;
	nodeColor.r = 255;
	nodeColor.g = 255;
	nodeColor.b = 20;
	GameEngine::Color connectionColor;
	connectionColor.r = 20;
	connectionColor.g = 255;
	connectionColor.b = 20;
	for (int i = 0; i < nodes.size(); i++) {
		sb->drawLine(nodes[i]->position, nodes[i]->position + glm::vec2(10, 10), nodeColor, 1);
		for (int j = 0; j < nodes[i]->neighbors.size(); j++) {
			sb->drawLine(nodes[i]->position, nodes[i]->neighbors[j]->position, connectionColor, 1);
		}
	}
}

std::vector<glm::vec2> PathFinding::getPath(int _start, int _end)
{
	std::vector<glm::vec2> path;
	std::vector<Node*> queue;
	

	//reseting previous
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->previous = nullptr;
		nodes[i]->visited = false;
	}

	//adding the end node to the queue
	queue.push_back(nodes[_end]);
	queue[queue.size() - 1]->visited = true;
	
	bool startFound = false;
	//go through all neighbors of end node
	while (queue.size() && !startFound) {
		//go through all neighbors of first node in queue
		for (int i = 0; i < queue[0]->neighbors.size(); i++) {
			//if this neighbor hasn't been visited
			if (!queue[0]->neighbors[i]->visited) {
				//if it finds the start
				if (queue[0]->neighbors[i] == nodes[_start]) {
					nodes[_start]->previous = queue[0];
					startFound = true;
					break;
				}
				else {
					//else add neigbor to queue
					queue.push_back(queue[0]->neighbors[i]);
					queue[queue.size() - 1]->visited = true;
				}
				//etting the neighbor added to the queue's previous to the current node
				queue[0]->neighbors[i]->previous = queue[0];
			}
		}
		//remove first element from queue;
		queue.erase(queue.begin());
	}
	//going back through the nodes using previous and creating the path
	Node* current = nodes[_start];
	while (true) {
		path.push_back(current->position);
		if (current->previous) {
			current = current->previous;
		}
		else {
			break;
		}
	}
	//printing the output
	for (int i = 0; i < path.size(); i++) {
		printf("path: %f, %f\n", path[i].x, path[i].y);
	}
	return path;
}

