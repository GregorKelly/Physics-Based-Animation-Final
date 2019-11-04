#include <iostream>
#include <cmath>
#include "Force.h"
#include "Body.h"
#include "glm/ext.hpp"
glm::vec3 Force::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) {
	return glm::vec3();
}
/*
** GRAVITY
*/
glm::vec3 Gravity::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) {
	// complete . Should return the gravity force
	
	return m_gravity * mass;
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
}/*
** HOOKE ’S LAW
*/
glm::vec3 Hooke::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) {
	// complete

	// calculate the spring direction vector
	glm::vec3 direction = m_b2->getPos() - m_b1->getPos();
	// calculate the length of spring 
	float len = glm::length(direction);
	//unit vector
	glm::vec3 e = direction / len;

	float vel1 = glm::dot(m_b1->getVel(), e);
	float vel2 = glm::dot(m_b2->getVel(), e);
	
	//simple spring force
	float springDampener = -m_ks * (m_rest - len) - m_kd * (vel1-vel2);

	glm::vec3 f1 = springDampener * e;
	
	if (m_b2->getPos() == pos) {
		f1 = -f1;
	}
	return f1;
}