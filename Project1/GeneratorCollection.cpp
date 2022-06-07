#include "GeneratorCollection.h"

GeneratorCollection::GeneratorCollection(){}

GeneratorCollection::~GeneratorCollection() {}

void GeneratorCollection::init(GameEngine::SpriteBatch * _sb){
	sb = _sb;
}

void GeneratorCollection::add(glm::vec2 _position){
	Generator* tempGenerator = new Generator;
	tempGenerator->init(_position, sb);
	generatorVector.push_back(tempGenerator);
}

void GeneratorCollection::drawByIndex(int _index){
	generatorVector[_index]->draw();
}

glm::vec2 GeneratorCollection::getPosition(int _index)
{
	return generatorVector[_index]->getPosition();
}

void GeneratorCollection::run(){

}
