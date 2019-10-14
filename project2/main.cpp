#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/ext.hpp"

// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Particle.h"
#include "Body.h"

// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 20.0f));

	// create ground plane
	Mesh plane = Mesh::Mesh(Mesh::QUAD);
	// scale it up x5
	plane.scale(glm::vec3(10.0f, 5.0f, 10.0f));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	plane.setShader(lambert);

	// create particle
	Particle particle1;
	// translate and set shader file
	particle1.translate(glm::vec3(0.0f, 2.0f, 0.0f));
	particle1.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));

	// set Gravity
	//glm::vec3 Fg = glm::vec3(0.0f, -9.8f, 0.0f);

	// intialisation
	glm::vec3 r = glm::vec3(0.0f, 5.0f, 0.0f);
	particle1.setVel(glm::vec3(5.0f, 10.0f, 3.0f));
	particle1.setMass(1.0f);
	particle1.addForce(new Gravity());
	particle1.addForce(new Drag());
	particle1.addForce(new Force());

	// creating the cube
	glm::vec3 cube_Origin = glm::vec3(-5.0f, 0.0f, -5.0f);
	glm::vec3 cube_dimensions = glm::vec3(10.0f);

	// time
	GLfloat firstFrame = (GLfloat)glfwGetTime();

	// set elasticity of particle
	float elasticity = 0.85f;

	// air resistance variables
	glm::vec3 drag;
	float medDencity = 1.0f;
	float dragCoefficient = 0.00001f;
	float crossSectionalArea = 1.0f;

	// wind cylinder
	float cylinder_radius = 2.0f;
	glm::vec3 force_wind;


	//blow dryer cone
	glm::vec3 cone_force = glm::vec3(0.0f, 0.0f, 0.0f);
	float cone_baseRadius = 4.0f;
	float cone_height = 5.0f;
	
	// wibbly wobbly timey wimey stuff
	double t = 0.0;
	double deltaTime = 0.01;

	double currentTime = (GLfloat)glfwGetTime();
	double accumulator = 0.0;

	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		double newTime = (GLfloat)glfwGetTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		//integration
		while (accumulator >= deltaTime) {
			t += deltaTime;
			accumulator -= deltaTime;
			
			/*
			**	INTERACTION
			*/
			// Manage interaction
			app.doMovement(deltaTime);

			/*
			**	SIMULATION
			*/

			// collision detection in cube
			glm::vec3 position = particle1.getPos();
			/*
			//compute drag
			drag = -0.5 * medDencity * glm::dot(particle1.getVel(), particle1.getVel()) * dragCoefficient * crossSectionalArea * glm::normalize(particle1.getVel());

			// compute all forces
			glm::vec3 f = Fg;

			// compute acceleration
			glm::vec3 a = f / particle1.getMass() + drag;
			*/

			//cone stuff
			/*
			float particle_location = sqrt((position.x) * (position.x) + (position.z) * (position.z));
			float cone_ratioY = 1.0f - (position.y / cone_height);

			//blow dryer force calculations
			if (position.y <= cone_height) {
				if (particle_location <= cone_baseRadius - cone_ratioY) {

					double ratio = cone_baseRadius / cylinder_radius;
					glm::vec3 coneForce_vector = glm::vec3(position.x * ratio, cone_height, position.z * ratio);
					glm::vec3 coneForce_angle = glm::vec3(coneForce_vector - position);
					double wind_speed = 10;
					force_wind = coneForce_angle * wind_speed * cone_ratioY;

					a = f / particle1.getMass() + drag + force_wind;
				}
			}
			*/

			// integrate
			particle1.setAcc(particle1.applyForces(particle1.getPos(), particle1.getVel(), t, deltaTime));
			particle1.getVel() = particle1.getVel() + deltaTime * particle1.getAcc();
			r = r + deltaTime * particle1.getVel();

			// move particle
			particle1.setPos(r);

			// collision detection in cube
			position = particle1.getPos();

			//x collisions
			if (position.x < cube_Origin.x) {
				particle1.getVel().x = -particle1.getVel().x * elasticity;
				r.x = cube_Origin.x;
			}
			else if (position.x > cube_dimensions.x) {
				particle1.getVel().x = -particle1.getVel().x * elasticity;
				r.x = cube_dimensions.x;
			}
			//y collisions
			if (position.y < cube_Origin.y) {
				particle1.getVel().y = -particle1.getVel().y * elasticity;
				r.y = cube_Origin.y;

			}
			else if (position.y > cube_dimensions.y) {
				particle1.getVel().y = -particle1.getVel().y * elasticity;
				r.y = cube_dimensions.y;
			}
			//z collisions
			if (position.z < cube_Origin.z) {
				particle1.getVel().z = -particle1.getVel().z * elasticity;
				r.z = cube_Origin.z;
			}
			else if (position.z > cube_dimensions.z) {
				particle1.getVel().z = -particle1.getVel().z * elasticity;
				r.z = cube_dimensions.z;
			}
		}

		const double alpha = accumulator / deltaTime;



		/*
		**	RENDER
		*/
		// clear buffer
		app.clear();
		// draw groud plane
		app.draw(plane);
		// draw particles
		app.draw(particle1.getMesh());

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

