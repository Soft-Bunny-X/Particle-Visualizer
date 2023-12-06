#include "Particle.h"

Particle::Particle(glm::vec3 pos, glm::vec3 vel, int charge, float mass)
	:Position{ pos }, Velocity{ vel }, Charge{ charge }, Mass{mass}
{
}

Particle::Particle()
	:Particle(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), 0, 1)
{
}

glm::vec3 Particle::getPosition()
{
	return Position;
}
glm::vec3 Particle::getVelocity()
{
	return Velocity;
}
int Particle::getCharge()
{
	return Charge;
}
float Particle::getMass()
{
	return Mass;
}

glm::vec3 Particle::getAcceleration(double currentTime, float dTime, Field &eField, Field &bField, Field &gField)
{
	glm::vec3 newVelocity = Velocity + glm::cross(Velocity, bField.getFieldVector(currentTime, getPosition())) * (Charge / Mass) * dTime;
	return (glm::normalize(newVelocity) * glm::length(Velocity) - Velocity) + eField.getFieldVector(currentTime, getPosition()) * (Charge / Mass) * dTime + gField.getFieldVector(currentTime, getPosition()) * dTime;
}

void Particle::update(double currentTime, float dTime, Field& eField, Field& bField, Field& gField)
{
	Position += Velocity * dTime;
	Velocity += getAcceleration(currentTime, dTime, eField, bField, gField);
}

Particle::~Particle()
{
}