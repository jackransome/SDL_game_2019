#pragma once
#include <glm.hpp>

struct Path {
	glm::vec2 position;
	Path* next;
};