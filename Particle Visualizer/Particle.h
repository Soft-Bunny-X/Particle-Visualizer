#pragma once

#include <glm/glm.hpp>
#include "Field.h"


class Particle
{
public:
	Particle();

	Particle(glm::vec3 pos, glm::vec3 vel, int charge, float mass);

	glm::vec3 getPosition();
	glm::vec3 getVelocity();
	int getCharge();
	float getMass();
	glm::vec3 getAcceleration(double currentTime, float dTime, Field &eField, Field &bField, Field &gField);
	
	void update(double currentTime, float dTime, Field& eField, Field& bField, Field& gField);

	~Particle();
private:
	glm::vec3 Position, Velocity;
	int Charge;
	float Mass; //unit in AMU (atomic mass unit)
};

