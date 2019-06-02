
#include <math.h>
#include <iostream>
#include <string>
#include <thread>

#include <GameEngine/Errors.h>
#include <GameEngine/ResourceManager.h>

#include "MainGame.h"


//Constructor, just initializes private member variables
MainGame::MainGame() :
	_screenWidth(800),
	_screenHeight(800),
	_time(0.0f),
	_gameState(GameState::PLAY),
	_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight, 0, 0);
}

//Destructor
MainGame::~MainGame()
{
}

//This runs the game
void MainGame::run() {
	initSystems();


	//This only returns when the game ends
	gameLoop();
}
//Initialize SDL and Opengl and whatever else we need
void MainGame::initSystems() {

	GameEngine::init();

	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();

	spriteBatch.init();
	drawText.init(&spriteBatch);
	_fpsLimiter.init(_maxFPS);
	_camera.setScreenShakeIntensity(10);
	//initialising collections
	projectiles.init(&spriteBatch);
	walls.init(&spriteBatch);
	wallTurrets.init(&spriteBatch, &projectiles);
	enemyDrones.init(&spriteBatch, &projectiles);

	pathFinding.init(&spriteBatch);

	//initialising the player
	player.init(0, 0, &spriteBatch, &_camera, &projectiles, &wallTurrets);

	walls.addWall(100, 0, 50, 200);
	walls.addWall(200, 0, 50, 200);
	walls.addWall(0, 250, 3500, 50);

	
	pathFinding.addNode(glm::vec2(0,0));
	pathFinding.addNode(glm::vec2(200, 200));
	pathFinding.addNode(glm::vec2(300, 300));
	pathFinding.addNode(glm::vec2(400, 400));
	pathFinding.addNode(glm::vec2(500, 500));
	pathFinding.connectNodes(0, 1);
	pathFinding.connectNodes(1, 2);
	pathFinding.connectNodes(2, 3);
	pathFinding.connectNodes(3, 4);
	pathFinding.getPath(0, 4);

}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.addAttribute("test");
	_colorProgram.linkShaders();

}

//This is the main game loop for our program
void MainGame::gameLoop() {

	//Will loop until we set _gameState to EXIT
	while (_gameState != GameState::EXIT) {

		_fpsLimiter.begin();

		processInput();
		_time += 0.1;


		_camera.update();
		//std::cout << _fps << std::endl;
		updateGame();

		drawGame();

		_fps = _fpsLimiter.end();

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 1000) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
	}
}
void MainGame::updateGame() {

	player.handleInput(&_inputManager);

	glm::vec2 mousePos = _inputManager.getMouseCoords();
	glm::vec2 cameraPos = _camera.getPosition();
	//_inputManager.setMouseCoords(mousePos.x - cameraPos.x, mousePos.y - cameraPos.y);
	for (int i = 0; i < walls.getVectorSize(); i++) {
		//player wall collision;
		if (collisionDetection.isCheckRequired(player.getBoundingBox(), walls.getBoundingBox(i))) {
			collisionDetection.correctPosition(player.getBoundingBox(), walls.getBoundingBox(i));
		}
		//projectile wall collision;
		for (int j = 0; j < projectiles.getVectorSize(); j++) {
			if (collisionDetection.lineRectCollision(projectiles.getProjectile(j)->getPosition(), projectiles.getProjectile(j)->getLastPosition(), walls.getBoundingBox(i))) {
				projectiles.remove(j);
				j--;
			}
		}
		//wallTurret wall collision
		for (int j = 0; j < wallTurrets.getVectorSize(); j++) {
			if (collisionDetection.isCheckRequired(wallTurrets.getBoundingBox(j), walls.getBoundingBox(i))) {
				if (collisionDetection.correctPosition(wallTurrets.getBoundingBox(j), walls.getBoundingBox(i))) {
					wallTurrets.setToStatic(j);
				}
			}
		}
		//enemyDrone wall collision
		for (int j = 0; j < enemyDrones.getVectorSize(); j++) {
			if (collisionDetection.isCheckRequired(enemyDrones.getBoundingBox(j), walls.getBoundingBox(i))) {
				collisionDetection.correctPosition(enemyDrones.getBoundingBox(j), walls.getBoundingBox(i));
			}
		}
	}
	//wallTurret and enemyDrone targeting 
	wallTurrets.clearTargets();
	enemyDrones.clearTargets();
	for (int i = 0; i < wallTurrets.getVectorSize(); i++) {
		for (int j = 0; j < enemyDrones.getVectorSize(); j++) {
			//checking if a wall is beteen
			bool isBlocked = false;
			for (int k = 0; k < walls.getVectorSize(); k++) {
				if (collisionDetection.lineRectCollision(wallTurrets.getCenter(i), enemyDrones.getCenter(j), walls.getBoundingBox(k))) {
					isBlocked = true;
					break;
				}
			}
			if (!isBlocked) {
				wallTurrets.target(i, enemyDrones.getBoundingBox(j));
				enemyDrones.target(j, wallTurrets.getBoundingBox(i));
			}
		}
	}
	//enemyDrone player targeting
	for (int i = 0; i < enemyDrones.getVectorSize(); i++) {
		//checking if a wall is beteen
		bool isBlocked = false;
		for (int j = 0; j < walls.getVectorSize(); j++) {
			if (collisionDetection.lineRectCollision(enemyDrones.getCenter(i), player.getCenter(), walls.getBoundingBox(j))) {
				isBlocked = true;
				break;
			}
		}
		if (!isBlocked) {
			enemyDrones.target(i, player.getBoundingBox());
		}
	}

	for (int i = 0; i < projectiles.getVectorSize(); i++) {
		if (projectiles.getDamageType(i) == damageFriendly) {
			//projectile player collision
			if (collisionDetection.lineRectCollision(projectiles.getProjectile(i)->getPosition(), projectiles.getProjectile(i)->getLastPosition(), player.getBoundingBox())) {
				player.changeHealth(-projectiles.getDamage(i));
				projectiles.setToDelete(i, true);
			}
			//projectile wall turret collision
			for (int j = 0; j < wallTurrets.getVectorSize(); j++) {
				if (collisionDetection.lineRectCollision(projectiles.getProjectile(i)->getPosition(), projectiles.getProjectile(i)->getLastPosition(), wallTurrets.getBoundingBox(j))) {
					wallTurrets.changeHealth(j, -projectiles.getDamage(i));
					projectiles.setToDelete(i, true);
				}
			}
		}
		else if (projectiles.getDamageType(i) == damageEnemy) {
			//projectile enemyDrone collision
			for (int j = 0; j < enemyDrones.getVectorSize(); j++) {
				if (collisionDetection.lineRectCollision(projectiles.getProjectile(i)->getPosition(), projectiles.getProjectile(i)->getLastPosition(), enemyDrones.getBoundingBox(j))) {
					enemyDrones.changeHealth(j, -projectiles.getDamage(i));
					projectiles.setToDelete(i, true);
				}
			}
		}
	}

	player.calcNewPos();
	projectiles.run();
	wallTurrets.update();
	enemyDrones.update();

	_inputManager.lastMouseR = _inputManager.isKeyPressed(SDL_BUTTON_RIGHT);
	_inputManager.lastMouseL = _inputManager.isKeyPressed(SDL_BUTTON_LEFT);
	_inputManager.lastMouseM = _inputManager.isKeyPressed(SDL_BUTTON_MIDDLE);
}
//Processes input with SDL
void MainGame::processInput() {
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	glm::vec2 cameraPos = _camera.getPosition();

	glm::vec2 trueMouseCoords = _inputManager.getTrueMouseCoords();
	_inputManager.setMouseCoords(trueMouseCoords.x + cameraPos.x - _screenWidth / 2, -(trueMouseCoords.y - cameraPos.y - _screenHeight / 2));

	//_inputManager.lastMouseR = _inputManager.isKeyPressed(SDL_BUTTON_RIGHT);
	//_inputManager.lastMouseL = _inputManager.isKeyPressed(SDL_BUTTON_LEFT);
	//_inputManager.lastMouseM = _inputManager.isKeyPressed(SDL_BUTTON_MIDDLE);

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			cameraPos = _camera.getPosition();
			_inputManager.setTrueMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
	if (_inputManager.isKeyPressed(SDL_BUTTON_MIDDLE) && !_inputManager.lastMouseM) {
		enemyDrones.addEnemyDrone(_inputManager.getMouseCoords());
	}

}

//Draws the game using the almighty OpenGL
void MainGame::drawGame() {
	//Set the base depth to 1.0
	glClearDepth(1.0);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLineWidth(1);

	glm::mat4 camera = _camera.getCameraMatrix();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(-1, -1);
	glVertex2f(-1, 1);
	glVertex2f(1, 1);
	glVertex2f(1, -1);
	glEnd();
	//Enable the shader
	_colorProgram.use();
	//We are using texture unit 0
	glActiveTexture(GL_TEXTURE0);
	//Get the uniform location
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//Tell the shader that the texture is in texture unit 0
	glUniform1i(textureLocation, 0);

	//Set the camera matrixx
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	spriteBatch.begin();

	player.draw();
	walls.draw();
	projectiles.draw();
	wallTurrets.draw();
	enemyDrones.draw();

	glm::vec2 temp;
	for (int i = 0; i < enemyDrones.getVectorSize(); i++) {
		temp = enemyDrones.getCenter(i) + glm::vec2(0, 20);
		drawText.drawString(temp.x, temp.y, std::to_string(i), 1);
	}
	for (int i = 0; i < wallTurrets.getVectorSize(); i++) {
		temp = wallTurrets.getCenter(i) + glm::vec2(0, 20);
		drawText.drawString(temp.x, temp.y, std::to_string(i), 1);
	}
	pathFinding.draw();

	spriteBatch.end();

	spriteBatch.renderBatch();

	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);


	//disable the shader
	_colorProgram.unuse();

	//Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}