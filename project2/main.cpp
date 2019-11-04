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
#include "Force.h"

int taskChanger = 0;

bool corner(int row, int col, int clothSize) {
	if (row == 0 && col == 0) {
		return true;
	}
	else if (row == 0 && col == clothSize - 1) {
		return true;
	}
	else if (row == clothSize - 1 && col == clothSize - 1) {
		return true;
	}
	else if (row == clothSize - 1 && col == 0) {
		return true;
	}
	else {
		return false;
	}
}

void hooke_up(std::vector<std::vector<Particle*>> particle_matrix, int i, int j, float cloth_spring ,float cloth_dampener, float cloth_rest){
	Hooke* hookeUp = new Hooke(particle_matrix[i][j], particle_matrix[i-1][j], cloth_spring, cloth_dampener, cloth_rest);
	particle_matrix[i][j]->addForce(hookeUp);
}
void hooke_upRight(std::vector<std::vector<Particle*>> particle_matrix, int i, int j, float cloth_spring, float cloth_dampener, float cloth_rest) {
	Hooke* hookeupRight = new Hooke(particle_matrix[i][j], particle_matrix[i-1][j+1], cloth_spring, cloth_dampener, cloth_rest);
	particle_matrix[i][j]->addForce(hookeupRight);
}
void hooke_right(std::vector<std::vector<Particle*>> particle_matrix, int i, int j, float cloth_spring, float cloth_dampener, float cloth_rest) {
	Hooke* hookeRight = new Hooke(particle_matrix[i][j], particle_matrix[i][j+1], cloth_spring, cloth_dampener, cloth_rest);
	particle_matrix[i][j]->addForce(hookeRight);
}
void hooke_downRight(std::vector<std::vector<Particle*>> particle_matrix, int i, int j, float cloth_spring, float cloth_dampener, float cloth_rest){
	Hooke* hookeDownRight = new Hooke(particle_matrix[i][j], particle_matrix[i+1][j+1], cloth_spring, cloth_dampener, cloth_rest);
	particle_matrix[i][j]->addForce(hookeDownRight);
}
void hooke_down(std::vector<std::vector<Particle*>> particle_matrix, int i, int j, float cloth_spring, float cloth_dampener, float cloth_rest) {
	Hooke* hookeDown = new Hooke(particle_matrix[i][j], particle_matrix[i+1][j], cloth_spring, cloth_dampener, cloth_rest);
	particle_matrix[i][j]->addForce(hookeDown);
}
void hooke_downLeft(std::vector<std::vector<Particle*>> particle_matrix, int i, int j, float cloth_spring, float cloth_dampener, float cloth_rest) {
	Hooke* hookeDownLeft = new Hooke(particle_matrix[i][j], particle_matrix[i+1][j-1], cloth_spring, cloth_dampener, cloth_rest);
	particle_matrix[i][j]->addForce(hookeDownLeft);
}
void hooke_left(std::vector<std::vector<Particle*>> particle_matrix, int i, int j, float cloth_spring, float cloth_dampener, float cloth_rest) {
	Hooke* hookeLeft = new Hooke(particle_matrix[i][j], particle_matrix[i][j-1], cloth_spring, cloth_dampener, cloth_rest);
	particle_matrix[i][j]->addForce(hookeLeft);
}
void hooke_upLeft(std::vector<std::vector<Particle*>> particle_matrix, int i, int j, float cloth_spring, float cloth_dampener, float cloth_rest) {
	Hooke* hookeUpLeft = new Hooke(particle_matrix[i][j], particle_matrix[i-1][j-1], cloth_spring, cloth_dampener, cloth_rest);
	particle_matrix[i][j]->addForce(hookeUpLeft);
}

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


	int particle_num_rope = 5;
	std::vector<Particle> particle_rope;

	int particle_num_chain = 10;
	std::vector<Particle> particle_chain;

	int particle_num_chain_collision = 10;
	std::vector<Particle> particle_chain_collision;

	int particle_num_cloth = 10;
	std::vector<std::vector<Particle*>> particle_matrix;

	// initialisers for task 1
	for (int i = 0; i < particle_num_rope; ++i) {
		Particle p;
		particle_rope.push_back(p);
		particle_rope[i].setPos(glm::vec3(i + 1.0f, 4.0f, 0.0f));
		particle_rope[i].getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
		particle_rope[i].addForce(new Gravity());
		particle_rope[i].setMass(0.1f);
		particle_rope[i].setVel(glm::vec3(0.0f, 0.0f, 0.0f));
		particle_rope[i].setAcc(glm::vec3(0.0f));
	}
	for (int i = 1; i < particle_rope.size(); i++) {
		Hooke* hooke = new Hooke(&particle_rope[i], &particle_rope[i - 1], 5, 5, 0.5f);
		particle_rope[i].addForce(hooke);
		particle_rope[i - 1].addForce(hooke);
	}

	// initialisers for task 2
	for (int i = 0; i < particle_num_chain; ++i) {
		Particle p;
		particle_chain.push_back(p);
		particle_chain[i].setPos(glm::vec3(i + 1.0f, 4.0f, 0.0f));
		particle_chain[i].getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
		particle_chain[i].addForce(new Gravity());
		particle_chain[i].setMass(0.1f);
		particle_chain[i].setVel(glm::vec3(0.0f, 0.0f, 0.0f));
		particle_chain[i].setAcc(glm::vec3(0.0f));
	}
	for (int i = 1; i < particle_chain.size(); i++) {
		Hooke* hooke = new Hooke(&particle_chain[i], &particle_chain[i - 1], 10, 10, 0.5f);
		particle_chain[i].addForce(hooke);
		particle_chain[i - 1].addForce(hooke);
	}

	// initialisers for task 3
	for (int i = 0; i < particle_num_chain_collision; ++i) {
		Particle p;
		particle_chain_collision.push_back(p);
		particle_chain_collision[i].setPos(glm::vec3(i + 1.0f, 2.0f, 0.0f));
		particle_chain_collision[i].getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
		particle_chain_collision[i].addForce(new Gravity());
		particle_chain_collision[i].setMass(0.1f);
		particle_chain_collision[i].setVel(glm::vec3(0.0f, 0.0f, 0.0f));
		particle_chain_collision[i].setAcc(glm::vec3(0.0f));
	}
	for (int i = 1; i < particle_chain_collision.size(); i++) {
		Hooke* hooke = new Hooke(&particle_chain_collision[i], &particle_chain_collision[i - 1], 5, 5, 0.5f);
		particle_chain_collision[i].addForce(hooke);
		particle_chain_collision[i - 1].addForce(hooke);
	}

	// initialisers for task 4 (NOTE: rows first then collumns)
	float cloth_spring = 30.0f;
	float cloth_dampener = 30.0f;
	float cloth_rest = 0.5f;


	for (int i = 0; i < particle_num_cloth; i++) {
		// declare row
		std::vector<Particle*> row;
		for (int j = 0; j < particle_num_cloth; j++) {
			Particle* p = new Particle();
			row.push_back(p);
			row[j]->setPos(glm::vec3(-5.0f + j + 1.0f, 10.0f, -5.0f + i + 1.0f));
			row[j]->getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
			row[j]->setMass(1.0f);
			row[j]->setVel(glm::vec3(0.0f, 0.0f, 0.0f));
			row[j]->setAcc(glm::vec3(0.0f));
		}
		particle_matrix.push_back(row);
	}
	// set forces
	for (int i = 0; i < particle_num_cloth; i++) {
		for (int j = 0; j < particle_num_cloth; j++) {
			if (!corner(i, j, particle_num_cloth)) {
				particle_matrix[i][j]->addForce(new Gravity);
				// left side particles
				if (j == 0) {
					hooke_up(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_upRight(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_right(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_downRight(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_down(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
				}
				// top side particles
				else if (i == 0) {
					hooke_left(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_downLeft(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_down(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_downRight(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_right(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
				}
				// right side particles
				else if (j == particle_matrix.size() -1) {
					hooke_up(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_upLeft(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_left(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_downLeft(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_down(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
				}
				// bottom side particles
				else if (i == particle_matrix.size() - 1) {
					hooke_left(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_upLeft(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_up(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_upRight(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_right(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
				}
				// particles in the middle
				else {
					hooke_up(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_upRight(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_right(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_downRight(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_down(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_downLeft(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_left(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
					hooke_upLeft(particle_matrix, i, j, cloth_spring, cloth_dampener, cloth_rest);
				}
			}
		}
	}




	// time
	GLfloat firstFrame = (GLfloat)glfwGetTime();

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

			/*
			**	INTERACTION
			*/
			// Manage interaction
			app.doMovement(deltaTime);
			taskChanger = app.taskChanger();

			/*
			**	SIMULATION
			*/
			if (taskChanger == 1) {
				for (int i = 1; i < particle_rope.size(); i++) {
					//integration
					particle_rope[i].setAcc(particle_rope[i].applyForces(particle_rope[i].getPos(), particle_rope[i].getVel(), t, deltaTime));
					particle_rope[i].setVel(particle_rope[i].getVel() + deltaTime * particle_rope[i].getAcc());
					particle_rope[i].setPos(particle_rope[i].getPos() + deltaTime * particle_rope[i].getVel());
				}
			}

			if (taskChanger == 2) {
				for (int i = 1; i < particle_chain.size() - 1; i++) {
					//integration
					particle_chain[i].setAcc(particle_chain[i].applyForces(particle_chain[i].getPos(), particle_chain[i].getVel(), t, deltaTime));
					particle_chain[i].setVel(particle_chain[i].getVel() + deltaTime * particle_chain[i].getAcc());
					particle_chain[i].setPos(particle_chain[i].getPos() + deltaTime * particle_chain[i].getVel());
				}
			}

			if (taskChanger == 3) {
				for (int i = 1; i < particle_chain_collision.size() - 1; i++) {
					//integration
					particle_chain_collision[i].setAcc(particle_chain_collision[i].applyForces(particle_chain_collision[i].getPos(), particle_chain_collision[i].getVel(), t, deltaTime));
					particle_chain_collision[i].setVel(particle_chain_collision[i].getVel() + deltaTime * particle_chain_collision[i].getAcc());
					particle_chain_collision[i].setPos(particle_chain_collision[i].getPos() + deltaTime * particle_chain_collision[i].getVel());

					if (particle_chain_collision[i].getPos().y < 0) {
						particle_chain_collision[i].getVel().y = -particle_chain_collision[i].getVel().y;
						particle_chain_collision[i].getPos().y = 0;
					}
				}
			}

			if (taskChanger == 4) {
				for (int i = 0; i < particle_num_cloth; i++) {
					for (int j = 0; j < particle_num_cloth; j++) {
						particle_matrix[i][j]->setAcc(particle_matrix[i][j]->applyForces(particle_matrix[i][j]->getPos(), particle_matrix[i][j]->getVel(), t, deltaTime));
						particle_matrix[i][j]->setVel(particle_matrix[i][j]->getVel() + deltaTime * particle_matrix[i][j]->getAcc());
						particle_matrix[i][j]->setPos(particle_matrix[i][j]->getPos() + deltaTime * particle_matrix[i][j]->getVel());

						if (particle_matrix[i][j]->getPos().y < 0) {
							particle_matrix[i][j]->getVel().y = -particle_matrix[i][j]->getVel().y;
							particle_matrix[i][j]->getPos().y = 0;
						}
					}
				}
			}

			t += deltaTime;
			accumulator -= deltaTime;
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
		if (taskChanger == 1) {
			for (int i = 0; i < particle_rope.size(); i++)
			{
				app.draw(particle_rope[i].getMesh());
			}
		}
		if (taskChanger == 2) {
			for (int i = 0; i < particle_chain.size(); i++)
			{
				app.draw(particle_chain[i].getMesh());
			}
		}
		if (taskChanger == 3) {
			for (int i = 0; i < particle_chain_collision.size(); i++)
			{
				app.draw(particle_chain_collision[i].getMesh());
			}
		}
		if (taskChanger == 4) {
			for (int i = 0; i < particle_num_cloth; i++) {
				for (int j = 0; j < particle_num_cloth; j++) {
					app.draw(particle_matrix[i][j]->getMesh());
				}
			}
		}

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

