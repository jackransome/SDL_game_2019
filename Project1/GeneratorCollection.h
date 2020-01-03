#pragma once
#include <vector>

#include "Generator.h"

class GeneratorCollection {
public:
	GeneratorCollection();
	~GeneratorCollection();
	void init(GameEngine::SpriteBatch* _sb);
	void add(glm::vec2 _position);
	void draw();
	void drawByIndex(int _index);
	glm::vec2 getPosition(int _index);
	void run();
	int getVectorSize() { return generatorVector.size(); }
	float getHealth(int _index);
	void remove(int _index);
private:
	GameEngine::SpriteBatch* sb;
	std::vector<Generator*> generatorVector;
};