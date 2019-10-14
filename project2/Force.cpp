#include <iostream>
#include <cmath>
#include "Force.h"
#include "Body.h"
#include "glm/ext.hpp"
glm::vec3 Force::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) {
	return glm::vec3(0.0f);
}
/*
** GRAVITY
*/
glm::vec3 Gravity::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) {
	// complete . Should return the gravity force
	return glm::vec3(0.0f, -9.8f, 0.0f);
}
/*
** DRAG
*/
glm::vec3 Drag::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) {
	// complete . Should return the aerodynamic drag force
	glm::vec3 drag;
	float medDencity = 1.0f;
	float dragCoefficient = 0.00001f;
	float crossSectionalArea = 1.0f;


	drag = -0.5 * medDencity * glm::dot(vel, vel) * dragCoefficient * crossSectionalArea * glm::normalize(vel);

	return drag;
}