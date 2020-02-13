#include "Generator.h"

Generator::Generator() {}

Generator::~Generator() {}

void Generator::init(glm::vec2 _position, GameEngine::SpriteBatch * _sb) {
	sb = _sb;
	boundingBox.x = _position.x;
	boundingBox.y = _position.y;
	boundingBox.w = 32;
	boundingBox.h = 32;
	boundingBox.xv = 0;
	boundingBox.yv = 0;
	onSprite.init(sb, 16, 24, 2, 11, 16, 0, 0);
	onSprite.loadTexture("textures/generatorOn.png");
	offSprite.init(sb, 16, 24, 2, 1, 16, 0, 0);
	offSprite.loadTexture("textures/generatorOff.png");
	health = 10;
	powerLevel = 0;
}

void Generator::draw() {
	if (powerLevel > 0) {
		onSprite.draw(boundingBox.x, boundingBox.y);
		onSprite.run();
	}
	else {
		offSprite.draw(boundingBox.x, boundingBox.y);
		offSprite.run();
	}
}

float Generator::getPowerLevel()
{
	return powerLevel;
}

void Generator::addPower(float _amount){
	powerLevel += _amount;
}

glm::vec2 Generator::getPosition() {
	return glm::vec2(boundingBox.x, boundingBox.y);
}
