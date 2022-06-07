#include "PathFinding.h"
#include <stdio.h>

void PathFinding::init(GameEngine::SpriteBatch * _sb)
{
	sb = _sb;
}

void PathFinding::clearNodeData()
{
	for (int i = 0; i < nodes.size(); i++) {
		free(nodes[i]->neighbors);
	}
	nodes.clear();
}

void PathFinding::addNode(glm::vec2 _position)
{
	nodes.push_back(new Node);
	nodes[nodes.size() - 1]->position = _position;
	
}

// Can only be used in the setup of the network as this acts on the vector not array
void PathFinding::connectNodes(int _index1, int _index2)
{
	nodes[_index1]->neighborsVector.push_back(nodes[_index2]);
	nodes[_index1]->numberOfNeighbors++;
	nodes[_index2]->neighborsVector.push_back(nodes[_index1]);
	nodes[_index2]->numberOfNeighbors++;
}
// Can only be used in the setup of the network as this acts on the vector not array
void PathFinding::disconnectNodes(int _index1, int _index2)
{
	//go through index1 and find index 2
	for (int i = 0; i < nodes[_index1]->numberOfNeighbors; i++) {
		if (nodes[_index1]->neighborsVector[i] == nodes[_index2]) {
			nodes[_index1]->neighborsVector.erase(nodes[_index1]->neighborsVector.begin() + i);
			break;
		}
	}
	//go through index2 and find index 1
	for (int i = 0; i < nodes[_index2]->numberOfNeighbors; i++) {
		if (nodes[_index2]->neighborsVector[i] == nodes[_index1]) {
			nodes[_index2]->neighborsVector.erase(nodes[_index2]->neighborsVector.begin() + i);
			break;
		}
	}
}

void PathFinding::optimiseNetwork()
{
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < i; j++) {
			for (int k = 0; k < j; k++) {
				if (isConnectedTo(i, j) && isConnectedTo(j, k) && isConnectedTo(k, i)) {
					float lengthij = sqrt(pow(nodes[i]->position.x - nodes[j]->position.x, 2) + pow(nodes[i]->position.y - nodes[j]->position.y, 2));
					float lengthjk = sqrt(pow(nodes[j]->position.x - nodes[k]->position.x, 2) + pow(nodes[j]->position.y - nodes[k]->position.y, 2));
					float lengthki = sqrt(pow(nodes[k]->position.x - nodes[i]->position.x, 2) + pow(nodes[k]->position.y - nodes[i]->position.y, 2));
					if (lengthij > lengthjk + lengthki) {
						disconnectNodes(i, j);
					}
					else if (lengthjk > lengthki + lengthij) {
						disconnectNodes(j, k);
					}
					else if (lengthki > lengthij + lengthjk) {
						disconnectNodes(k, i);
					}
					else {
						if (lengthij > lengthjk) {
							if (lengthij > lengthki) {
								disconnectNodes(i, j);
							}
							else {
								disconnectNodes(k, i);
							}
						}
						else {
							if (lengthjk > lengthki) {
								disconnectNodes(j, k);
							}
							else {
								disconnectNodes(k, i);
							}
						}
					}
				}
			}
		}
	}
}

bool PathFinding::isConnectedTo(int _index1, int _index2)
{
	for (int i = 0; i < nodes[_index1]->numberOfNeighbors; i++) {
		if (nodes[_index1]->neighbors[i] == nodes[_index2]) {
			return true;
		}
	}
	return false;
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
		for (int j = 0; j < nodes[i]->numberOfNeighbors; j++) {
			sb->drawLine(nodes[i]->position, nodes[i]->neighbors[j]->position, connectionColor, 1);
		}
	}
}

void PathFinding::fillNeighbors()
{
	// Looping through all nodes and filling their neighbors array using their neighbors vector, then removing the vector
	for (int i = 0; i < nodes.size(); i++) {
		// Allocating the memory for the neighbors array
		nodes[i]->neighbors = (Node**)malloc(sizeof(Node*) * nodes[i]->numberOfNeighbors);
		// Filling the neighbors array
		for (int j = 0; j < nodes[i]->numberOfNeighbors; j++){
			nodes[i]->neighbors[j] = nodes[i]->neighborsVector[j];
		}
		// Allocating the memory for the distance array
		nodes[i]->distances = (int*)malloc(sizeof(int) * nodes[i]->numberOfNeighbors);
		// Filling distance array
		for (int j = 0; j < nodes[i]->numberOfNeighbors; j++) {
			nodes[i]->distances[j] = sqrt(pow(nodes[i]->position.x - nodes[i]->neighbors[j]->position.x, 2) + pow(nodes[i]->position.y - nodes[i]->neighbors[j]->position.y, 2));
		}
		// Ordering neighbors/distance array based on distance from the node (bubble sort)
		Node* tempNodePointer;
		int tempDistance;
		for (int j = 0; j < nodes[i]->numberOfNeighbors - 1; j++) {
			for (int k = 0; k < nodes[i]->numberOfNeighbors - j - 1; k++) {
				if (nodes[i]->distances[k] > nodes[i]->distances[k+1]) {
					// swap neighbors k and k +1 in both distances and neighbors
					tempDistance = nodes[i]->distances[k];
					tempNodePointer = nodes[i]->neighbors[k];
					nodes[i]->distances[k] = nodes[i]->distances[k+1];
					nodes[i]->neighbors[k] = nodes[i]->neighbors[k+1];
					nodes[i]->distances[k+1] = tempDistance;
					nodes[i]->neighbors[k+1] = tempNodePointer;
				}
			}
		}
		// Emptying the vector:
		nodes[i]->neighborsVector.clear();
	}
	
}

Path* PathFinding::getPath(int _start, int _end)
{
	// check if cached path for _start exist
	if (nodes[_start]->cachedPath) {
		// check if the cached path leads to _end, if so, return it
		Path* curr = nodes[_start]->cachedPath;
		while (curr) {
			// if _end is found in the path return it (not an issue if it's the last node)
			if (curr->position == nodes[_end]->position) {
				return nodes[_start]->cachedPath;
			}
			curr = curr->next;
		}
	}

	std::vector<Node*> queue;
	

	//reseting previous and visited
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
		for (int i = 0; i < queue[0]->numberOfNeighbors; i++) {
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
				//getting the neighbor added to the queue's previous to the current node
				queue[0]->neighbors[i]->previous = queue[0];
			}
		}
		//remove first element from queue
		queue.erase(queue.begin());
	}
	//going back through the nodes using previous and creating the path
	Node* current = nodes[_start];

	Path* path = new Path;

	Path* currentPath = path;
	Path* previousPath = path;

	while (true) {
		if (previousPath != currentPath) {
			previousPath->next = currentPath;
		}
		currentPath->position = current->position;
		previousPath = currentPath;
		currentPath = new Path;
		if (current->previous) {
			current = current->previous;
		}
		else {
			break;
		}
	}
	previousPath->next = NULL;
	nodes[_start]->cachedPath = path;
	return path;
}

glm::vec2 PathFinding::getPosition(int _index)
{
	return nodes[_index]->position;
}

