#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <vector>

#include <GameEngine/GameEngine.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/GLTexture.h>
#include <GameEngine/Sprite.h>
#include <GameEngine/Window.h>
#include <GameEngine/InputManager.h>
#include <GameEngine/Timing.h>
#include <GameEngine/CollisionDetection.h>
#include <GameEngine/SpriteBatch.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/DrawText.h>
#include <GameEngine/Sound.h>

#include "Player.h"
#include "WallCollection.h"
#include "ProjectileCollection.h"
#include "WallTurretCollection.h"
#include "GeneratorCollection.h"
#include "EnemyDroneCollection.h"
#include "PathFinding.h"
#include "RandomGeneration.h"

enum class GameState { PLAY, EXIT };

// enum used for keeping track of the order of drawing
enum CollectionType { wallTurretC, projectileC, playerNoC, enemyDroneC, wallC, generatorC};

// object used for keeping track of the order of drawing
struct drawObject {
	CollectionType collectionType;
	int collectionIndex;
	int yValue;
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
private:

	int closestNodeToPlayerIndex;

	bool playerNodeHasChanged;

	std::vector<drawObject> drawObjectCollection;

	std::vector<glm::vec2> tempPath;

	Player player;

	ProjectileCollection projectiles;

	WallCollection walls;

	WallTurretCollection wallTurrets;

	EnemyDroneCollection enemyDrones;

	GeneratorCollection generators;

	PathFinding pathFinding;
	
	RandomGeneration randomGeneration;

	GameEngine::CollisionDetection collisionDetection;
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	void updateGame();
	GameEngine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	GameEngine::GLSLProgram _colorProgram;
	GameEngine::Camera2D camera;

	GameEngine::SpriteBatch spriteBatch;

	GameEngine::DrawText drawText;

	GameEngine::InputManager inputManager;
	GameEngine::FpsLimiter fpsLimiter;

	GameEngine::Sound sound;

	float _maxFPS;
	float _fps;
	float _time;
	bool lastPressedL = false;
	bool lastPressedR = false;
};
