
#include <SDL.h>

#include "Player.h"

Player::Player()
{
}
Player::~Player()
{
}
void Player::init(float _x, float _y, GameEngine::SpriteBatch* _sb, GameEngine::Camera2D* _c) {
	sb = _sb;
	camera = _c;
	boundingBox.x = _x;
	boundingBox.y = _y;
	boundingBox.w = 32;
	boundingBox.h = 32;
	boundingBox.xv = 0;
	boundingBox.yv = 0;
	speed = 4;
	health = 10;
	mainSprite.init(sb, 16, 16, 2, 4, 16, 0, 0);
	mainSprite.loadTexture("textures/player1.png");
}
BoundingBox* Player::getBoundingBox() {
	return &boundingBox;
}
void Player::handleInput(GameEngine::InputManager* _im) {
	camera->followObject(&boundingBox);
	boundingBox.yv = boundingBox.xv = 0;

	if (_im->isKeyPressed(SDLK_w)) {
		boundingBox.yv = 1;
	}
	if (_im->isKeyPressed(SDLK_a)) {
		boundingBox.xv = -1;
	}
	if (_im->isKeyPressed(SDLK_s)) {
		boundingBox.yv = -1;
	}
	if (_im->isKeyPressed(SDLK_d)) {
		boundingBox.xv = 1;
	}
	if (abs(boundingBox.xv) + abs(boundingBox.yv) == 2) {
		boundingBox.xv *= sqrt(0.5);
		boundingBox.yv *= sqrt(0.5);
	}

	boundingBox.yv *= speed;
	boundingBox.xv *= speed;

}
void Player::calcNewPos() {
	boundingBox.x += boundingBox.xv;
	boundingBox.y += boundingBox.yv;
}
float Player::getHealth()
{
	return health;
}
void Player::changeHealth(float _amount)
{
	health += _amount;
}
glm::vec2 Player::getCenter()
{
	return glm::vec2(boundingBox.x + boundingBox.w / 2, boundingBox.y + boundingBox.h / 2);
}
void Player::draw() {
	mainSprite.run();
	mainSprite.draw(boundingBox.x, boundingBox.y);
}