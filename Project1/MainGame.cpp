
#include <math.h>
#include <iostream>
#include <string>
#include <thread>

#include <GameEngine/Errors.h>
#include <GameEngine/ResourceManager.h>

#include "MainGame.h"


//Constructor, just initializes private member variables
MainGame::MainGame() :
	_screenWidth(1920),
	_screenHeight(1080),
	_time(0.0f),
	_gameState(GameState::PLAY),
	_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight, 0, 0);
}

MainGame::~MainGame()
{
}

//This runs the game
void MainGame::run() {
	initSystems();


	//This only returns when the game ends
	gameLoop();

	sound.unloadAll();

	pathFinding.clearNodeData();
}
//Initialize SDL and Opengl and whatever else we need
void MainGame::initSystems() {
	srand(time(NULL));
	GameEngine::init();

	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();

	spriteBatch.init();
	drawText.init(&spriteBatch);
	_fpsLimiter.init(_maxFPS);
	_camera.setScreenShakeIntensity(0);

	//initialising collections
	projectiles.init(&spriteBatch);
	walls.init(&spriteBatch);
	wallTurrets.init(&spriteBatch, &projectiles, &_camera);
	enemyDrones.init(&spriteBatch, &projectiles, &_camera);
	randomGeneration.init(&walls, &pathFinding, &collisionDetection);

	pathFinding.init(&spriteBatch);

	//initialising the player
	player.init(0, 0, &spriteBatch, &_camera, &projectiles, &wallTurrets);
	bool collides = false;

	bool random = true;
	int amountOfWalls = 30;
	if (!random) {
		walls.addWall(100, 0, 50, 200);
		walls.addWall(200, 0, 50, 200);
		walls.addWall(0, 250, 3500, 50);
	}
	else {
		//randomly generating walls
		/*while (walls.getVectorSize() < amountOfWalls) {
			walls.addWall(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 200 + 50, rand() % 200 + 50);
			for (int i = 0; i < walls.getVectorSize()-1; i++) {
				BoundingBox temp1 = *walls.getBoundingBox(walls.getVectorSize() - 1);
				temp1.x -= 32;
				temp1.y -= 32;
				temp1.w += 64;
				temp1.h += 64;
				if (collisionDetection.CheckRectangleIntersect(&temp1, walls.getBoundingBox(i))) {
					walls.remove(walls.getVectorSize() - 1);
					break;
				}
			}
		}*/
		randomGeneration.generate();
	}

	/*for (int i = 0; i < walls.getVectorSize(); i++) {
		BoundingBox* temp = walls.getBoundingBox(i);
		for (int j = 0; j < walls.getVectorSize(); j++) {
			if (collisionDetection.pointRectangleIntersect(glm::vec2(temp->x - 20, temp->y - 20), walls.getBoundingBox(j))) {
				collides = true;
			}
		}
		if (!collides) {
			pathFinding.addNode(glm::vec2(temp->x - 20, temp->y - 20));
		}
		collides = false;
		for (int j = 0; j < walls.getVectorSize(); j++) {
			if (collisionDetection.pointRectangleIntersect(glm::vec2(temp->x - 10, temp->y + temp->h + 20), walls.getBoundingBox(j))) {
				collides = true;
			}
		}
		if (!collides) {
			pathFinding.addNode(glm::vec2(temp->x - 20, temp->y + temp->h + 20));
		}
		collides = false;
		for (int j = 0; j < walls.getVectorSize(); j++) {
			if (collisionDetection.pointRectangleIntersect(glm::vec2(temp->x + temp->w + 20, temp->y + temp->h + 20), walls.getBoundingBox(j))) {
				collides = true;
			}
		}
		if (!collides) {
			pathFinding.addNode(glm::vec2(temp->x + temp->w + 20, temp->y + temp->h + 20));
		}
		collides = false;
		for (int j = 0; j < walls.getVectorSize(); j++) {
			if (collisionDetection.pointRectangleIntersect(glm::vec2(temp->x + temp->w + 20, temp->y - 20), walls.getBoundingBox(j))) {
				collides = true;
			}
		}
		if (!collides) {
			pathFinding.addNode(glm::vec2(temp->x + temp->w + 20, temp->y - 20));
		}
		
	}*/
	//connecting nodes that can reach each other
	
	for (int i = 0; i < pathFinding.getVectorSize(); i++) {
		for (int j = i; j < pathFinding.getVectorSize(); j++) {
			collides = false;
			for (int k = 0; k < walls.getVectorSize(); k++) {
				if (i != j) {
					glm::vec2 temp1 = pathFinding.getPosition(i);
					glm::vec2 temp2 = pathFinding.getPosition(j);
					BoundingBox* temp3 = walls.getBoundingBox(k);
					bool result = collisionDetection.lineRectCollision(temp1, temp2, walls.getBoundingBox(k));
					if (result) {
						collides = true;
						break;
					}
				}
			}
			if (!collides && i != j) { // HERE
				pathFinding.connectNodes(i, j);
			}
		}
	}
	pathFinding.fillNeighbors();
	//pathFinding.optimiseNetwork();
	int x = 0;
	int j = sound.loadSound("trac1.wav");
	sound.playSound(j);
	sound.loopSound(j); 
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

	//get closest node to player that the player can see
	float minDistance = 1000000;
	int index = -1;
	float dist;
	for (int i = 0; i < pathFinding.getVectorSize(); i++) {
		for (int j = 0; j < walls.getVectorSize(); j++) {
			bool result = collisionDetection.lineRectCollision(player.getCenter(), pathFinding.getPosition(i), walls.getBoundingBox(j));
			if (!result) {
				dist = collisionDetection.getDistance(player.getCenter(), pathFinding.getPosition(i));
				if (dist < minDistance) {
					index = i;
					minDistance = dist;
				}
			}
		}
	}
	if (closestNodeToPlayerIndex != index) {
		closestNodeToPlayerIndex = index;
		playerNodeHasChanged = true;
	}
	else {
		playerNodeHasChanged = false;
	}
	

	//tempPath = pathFinding.getPath(0, closestNodeToPlayerIndex)

	// drone drone collision
	/*for (int i = 0; i < enemyDrones.getVectorSize(); i++) {
		for (int j = 0; j < enemyDrones.getVectorSize(); j++) {
			if (j != i && collisionDetection.isCheckRequired(enemyDrones.getBoundingBox(i), enemyDrones.getBoundingBox(j))) {
				collisionDetection.correctPosition(enemyDrones.getBoundingBox(i), enemyDrones.getBoundingBox(j));
			}
		}
	}*/
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
			if (!wallTurrets.getStatic(j) && collisionDetection.isCheckRequired(wallTurrets.getBoundingBox(j), walls.getBoundingBox(i))) {
				if (collisionDetection.isCheckRequired(wallTurrets.getBoundingBox(j), walls.getBoundingBox(i)) && collisionDetection.correctPosition(wallTurrets.getBoundingBox(j), walls.getBoundingBox(i))) {
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
				if (sqrt(pow(enemyDrones.getCenter(j).x - wallTurrets.getCenter(i).x, 2) + pow(enemyDrones.getCenter(j).y - wallTurrets.getCenter(i).y, 2)) < enemyDrones.getSensorRange(j)) {
					enemyDrones.target(j, wallTurrets.getBoundingBox(i));
				}
				if (sqrt(pow(wallTurrets.getCenter(i).x - enemyDrones.getCenter(j).x, 2) + pow(wallTurrets.getCenter(i).y - enemyDrones.getCenter(j).y, 2)) < wallTurrets.getSensorRange(i)) {
					wallTurrets.target(i, enemyDrones.getBoundingBox(j));
				}
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
		//if isn't blocked and if within sensor range
		if (!isBlocked && sqrt(pow(enemyDrones.getCenter(i).x - player.getCenter().x, 2) + pow(enemyDrones.getCenter(i).y - player.getCenter().y, 2)) < enemyDrones.getSensorRange(i)) {
			enemyDrones.target(i, player.getBoundingBox());
		}
		//if a drone doesn't have a path or the path to the player has changed
		else if ((!enemyDrones.hasPath(i) || playerNodeHasChanged) && !enemyDrones.hasTarget(i)) {
			//slow
			float minDistance = 1000000;
			int index = -1;
			float dist;
			bool collided;
			//getting the closest node to the drone that it can see
			for (int j = 0; j < pathFinding.getVectorSize(); j++) {
				collided = false;
				for (int k = 0; k < walls.getVectorSize(); k++) {
					bool result = collisionDetection.lineRectCollision(enemyDrones.getCenter(i), pathFinding.getPosition(j), walls.getBoundingBox(k));
					if (result) {
						collided = true;
					}
				}
				if (!collided) {
					dist = collisionDetection.getDistance(enemyDrones.getCenter(i), pathFinding.getPosition(j));
					if (dist < minDistance) {
						index = j;
						minDistance = dist;
					}
				}
			}
			if (index != -1) {
				enemyDrones.setPath(i, pathFinding.getPath(index, closestNodeToPlayerIndex));
			}
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
	if (_inputManager.isKeyPressed(SDL_BUTTON_MIDDLE)/* && !_inputManager.lastMouseM*/) {
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

	//pathFinding.draw();

	// add all drawable objects to the drawObjectCollection vector
	for (int i = 0; i < walls.getVectorSize(); i++) {
		drawObject temp;
		temp.collectionType = wallC;
		temp.collectionIndex = i;
		temp.yValue = walls.getBoundingBox(i)->y;
		drawObjectCollection.push_back(temp);
	}

	for (int i = 0; i < wallTurrets.getVectorSize(); i++) {
		drawObject temp;
		temp.collectionType = wallTurretC;
		temp.collectionIndex = i;
		temp.yValue = wallTurrets.getBoundingBox(i)->y;
		drawObjectCollection.push_back(temp);
	}

	for (int i = 0; i < enemyDrones.getVectorSize(); i++) {
		drawObject temp;
		temp.collectionType = enemyDroneC;
		temp.collectionIndex = i;
		temp.yValue = enemyDrones.getBoundingBox(i)->y;
		drawObjectCollection.push_back(temp);
	}

	for (int i = 0; i < projectiles.getVectorSize(); i++) {
		drawObject temp;
		temp.collectionType = projectileC;
		temp.collectionIndex = i;
		temp.yValue = projectiles.getPosition(i).y;
		drawObjectCollection.push_back(temp);
	}

	drawObject temp;
	temp.collectionType = playerNoC;
	temp.collectionIndex = 0;
	temp.yValue = player.getBoundingBox()->y;
	drawObjectCollection.push_back(temp);

	// order drawObjectCollection in terms of y value (largest y first)

	for (int i = 0; i < drawObjectCollection.size() - 1; i++) {
		for (int j = 0; j < drawObjectCollection.size() - i - 1; j++) {
			if (drawObjectCollection[j].yValue < drawObjectCollection[j + 1].yValue) {
				temp = drawObjectCollection[j];
				drawObjectCollection[j] = drawObjectCollection[j + 1];
				drawObjectCollection[j + 1] = temp;
			}
		}
	}

	// draw all objects in drawObjectCollection

	for (int i = 0; i < drawObjectCollection.size(); i++) {
		switch (drawObjectCollection[i].collectionType) {
		case wallC:
			walls.drawByIndex(drawObjectCollection[i].collectionIndex);
			break;
		case wallTurretC:
			wallTurrets.drawByIndex(drawObjectCollection[i].collectionIndex);
			break;
		case enemyDroneC:
			enemyDrones.drawByIndex(drawObjectCollection[i].collectionIndex);
			break;
		case projectileC:
			projectiles.drawByIndex(drawObjectCollection[i].collectionIndex);
			break;
		case playerNoC:
			player.draw();
			break;
		}
	}
	drawObjectCollection.clear();
	/*player.draw();
	walls.draw();
	projectiles.draw();
	wallTurrets.draw();
	enemyDrones.draw();
	*/

	spriteBatch.end();

	spriteBatch.renderBatch();

	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);


	//disable the shader
	_colorProgram.unuse();

	//Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}