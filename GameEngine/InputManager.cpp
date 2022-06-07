#include "InputManager.h"
#include <iostream>
namespace GameEngine {

	InputManager::InputManager() : _mouseCoords(0)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::pressKey(unsigned int keyID) {
		// Here we are treating keyMap as an associative array.
		// if keyID doesn't already exist in keyMap, it will get added
		keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID) {
		keyMap[keyID] = false;
	}

	void InputManager::setOldKeyMap(){
		oldKeyMap = keyMap;
	}

	void InputManager::setMouseCoords(float x, float y) {
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}
	void InputManager::setTrueMouseCoords(float x, float y) {
		_trueMouseCoords.x = x;
		_trueMouseCoords.y = y;
	}
	void InputManager::addMouseCoords(glm::vec2 _pos) {
		//_mouseCoords.x *= -1;
		//std::cout << "xy: " << _mouseCoords.x << ", " << _mouseCoords.y << std::endl;
	}
	bool InputManager::isKeyPressed(unsigned int keyID) {
		// We dont want to use the associative array approach here
		// because we don't want to create a key if it doesnt exist.
		// So we do it manually
		auto it = keyMap.find(keyID);
		if (it != keyMap.end()) {
			// Found the key
			return it->second;
		}
		else {
			// Didn't find the key
			return false;
		}
	}
	bool InputManager::wasKeyPressed(unsigned int keyID) {
		// We dont want to use the associative array approach here
		// because we don't want to create a key if it doesnt exist.
		// So we do it manually
		auto it = oldKeyMap.find(keyID);
		if (it != oldKeyMap.end()) {
			// Found the key
			return it->second;
		}
		else {
			// Didn't find the key
			return false;
		}
	}

	glm::vec2 InputManager::getMouseCoords()
	{
		return _mouseCoords;
	}

	glm::vec2 InputManager::getTrueMouseCoords()
	{
		return _trueMouseCoords;

	}
}