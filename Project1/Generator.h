#pragma once

#include "GameEngine/SpriteSheet.h"
#include "GameEngine/SpriteBatch.h"
#include <GameEngine/BoundingBox.h>
#include <GameEngine/GameEngine.h>

class Generator {
public:
	Generator();
	~Generator();
	void init(glm::vec2 _position, GameEngine::SpriteBatch* _sb);
	void draw();
	float getPowerLevel();
	void addPower(float _amount);
	glm::vec2 getPosition();
private:
	float powerLevel;
	int health;
	GameEngine::SpriteBatch * sb;
	BoundingBox boundingBox;
	GameEngine::SpriteSheet onSprite;
	GameEngine::SpriteSheet offSprite;
};