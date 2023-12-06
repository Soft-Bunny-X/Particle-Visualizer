#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <fstream>
#include <iterator>
#include <random>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Particle.h"
#include "Field.h"
#include "Sawtooth.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void AutoGenParticles(std::vector <Particle> &particleList, int xNum, int yNum, int zNum, float spacing, glm::vec3 initPos, bool randomInitVel, glm::vec3 initVel, double speedScale, int charge, float mass)
{
	glm::vec3 currentPos;
	//float minVel{0.01};

	for (size_t x = 0; x < xNum; x++) {
		for (size_t y = 0; y < yNum; y++) {
			for (size_t z = 0; z < zNum; z++) {
				if (!randomInitVel) {
					currentPos = initPos + glm::vec3(x * spacing, y * spacing, z * spacing);
					particleList.push_back(Particle(currentPos, initVel, charge, mass));
				}
				else{
					double pi = 2 * asin(1.0);
					std::random_device rd;
					std::mt19937 gen(rd());
					std::fisher_f_distribution<double> speedDistribution(20.0, 20.0);
					std::uniform_real_distribution<double> vecComponentDistrib(0.0, 2.0);
					double theta{ pi * vecComponentDistrib(gen) / 2 };
					double phi{ pi * vecComponentDistrib(gen) };
					initVel.x = speedScale * sin(theta) * cos(phi) * speedDistribution(gen);
					initVel.y = speedScale * sin(theta) * sin(phi) * speedDistribution(gen);
					initVel.z = speedScale * cos(theta) * speedDistribution(gen);

					currentPos = initPos + glm::vec3(x * spacing, y * spacing, z * spacing);
					particleList.push_back(Particle(currentPos, initVel, charge, mass));
				}
			}
		}
	}
}

void ParticleGenerator(std::vector <Particle>& particleList) {
	std::string defaultParticles{""};

	int charge{0};
	float mass{1.0};

	std::cout << "========================================\n";
	std::cout << "========================================\n";
	std::cout << "\nGenerate default particle configuration? (Y/N)\n";
	std::cin >> defaultParticles;
	if (defaultParticles == "y" || defaultParticles == "Y") {
		AutoGenParticles(particleList, 10, 10, 10, 0.2, glm::vec3(-1, -1, -1), true, glm::vec3(0.0, 0.0, 0.0), 1, 1, 1);
	}
	else {
		std::string fromFile{""};

		std::cout << "========================================\n";
		std::cout << "========================================\n";
		std::cout << "\nGenerate particles from file? (Y/N)\n";
		std::cin >> fromFile;
		if (fromFile == "Y" || fromFile == "y") {
			std::string fileName{ "Initialization Data/particles.csv" };

			/*std::cout << "\nEnter the filename of the particle parameter file:\n";
			std::cin >> fileName;*/

			std::fstream in_file{ fileName, std::ios::in };
			if (in_file.is_open()) {
				std::string line;
				std::getline(in_file, line);
				while (!in_file.eof()) {
					std::getline(in_file, line);
					char prevChar{ ' ' };
					std::string tempEntry{ "" };
					std::vector<float> paraList{};

					for (auto i = line.begin(); i != line.end(); ++i) {
						if ((prevChar == ' ' || prevChar == ',') && (*i >= '0' && *i <= '9')) {
							tempEntry = "";
						}
						else if ((*i == ' ' || *i == ',') && ((prevChar >= '0' && prevChar <= '9') || prevChar == '.')) {
							paraList.push_back(std::stof(tempEntry));
						}
						tempEntry.push_back(*i);
						prevChar = *i;
					}
					if (paraList.size() != 8) {
						throw std::runtime_error("File format error! Number of parameters doesn't match!");
					}

					glm::vec3 position{ paraList[0], paraList[1], paraList[2] };
					glm::vec3 velocity{ paraList[3], paraList[4], paraList[5] };

					particleList.push_back(Particle(position, velocity, (int)(paraList[6]), paraList[7]));
				}
			}
			else {
				throw std::runtime_error("File failed to open");
			}
			in_file.close();
		}
		else {
			std::cout << "\nEnter the charge of the particle: (1 for proton, 0 for neutron, -1 for electron)\n";
			std::cin >> charge;

			std::cout << "\nEnter the mass of the particle:\n";
			std::cin >> mass;

			if (mass == 0) {
				throw std::runtime_error("Mass couldn't be zero!");
			}

			/*std::cout << "\nEnter the number of particles along each direction:\n\nx:\n";
			std::cin >> xNum;
			std::cout << "\ny:\n";
			std::cin >> yNum;
			std::cout << "\nz:\n";
			std::cin >> zNum;*/

			int xNum, yNum, zNum;

			std::cout << "\nEnter the number of particles along each dimension: (for the number \'n\' entered, a lattice of n * n * n particles will be generated)\n";
			std::cin >> xNum;
			yNum = zNum = xNum;

			float spacing{ 1.0 };

			std::cout << "\nEnter the spacing between the particles: (in nm)\n";
			std::cin >> spacing;

			/*float x, y, z;

			std::cout << "\nEnter the position offset of the particle lattice:\n\nx:\n";
			std::cin >> x;
			std::cout << "\ny:\n";
			std::cin >> y;
			std::cout << "\nz:\n";
			std::cin >> z;*/

			/*float xVel, yVel, zVel;
			std::string randInitVel{""};

			std::cout << "\nRandom initial velocity for each particle? (Y/N)\n";
			std::cin >> randInitVel;

			if (randInitVel == "Y" || randInitVel == "y") {
				AutoGenParticles(particleList, xNum, yNum, zNum, spacing, glm::vec3(x, y, z), true, glm::vec3(0.0, 0.0, 0.0), charge, mass);
			}
			else {
				std::cout << "\nEnter the components of the uniform initial velocity:\n\nx:\n";
				std::cin >> xVel;
				std::cout << "\ny:\n";
				std::cin >> yVel;
				std::cout << "\nz:\n";
				std::cin >> zVel;
				AutoGenParticles(particleList, xNum, yNum, zNum, spacing, glm::vec3(x, y, z), false, glm::vec3(xVel, yVel, zVel), charge, mass);
			}*/

			double speedScale;

			std::cout << "\nEnter the velocity scale: (the larger this number, the higher particles' mean speed. This number is also an indication of temperature)\n";
			std::cin >> speedScale;
			AutoGenParticles(particleList, xNum, yNum, zNum, spacing, glm::vec3(-(xNum * spacing * 0.5), -(yNum * spacing * 0.5), -(zNum * spacing * 0.5)), true, glm::vec3(0.0, 0.0, 0.0), speedScale, charge, mass);
		}
	}
}

void particlesToVertices(std::vector<Particle> &particleList, GLfloat *vertices) {
	GLfloat* verticesIndex{ vertices };
	size_t vertexIndex{ 0 };
	for (auto i = particleList.begin(); i != particleList.end(); i++) {
		glm::vec3 vertexPos = (*i).getPosition();
		vertices[vertexIndex] = (GLfloat)vertexPos.x;
		vertices[vertexIndex + 1] = (GLfloat)vertexPos.y;
		vertices[vertexIndex + 2] = (GLfloat)vertexPos.z;
		vertexIndex += 3;
	}
}

void createAxis(Mesh *axisMesh) {
	unsigned int indices[] = {
		0, 1,
		2, 3,
		4, 5
	};

	GLfloat vertices[] = {
		100.0, 0.0, 0.0,
		-100.0, 0.0, 0.0,
		0.0, 100.0, 0.0,
		0.0, -100.0, 0.0,
		0.0, 0.0, 100.0,
		0.0, 0.0, -100.0
	};
	
	axisMesh->CreateMesh(vertices, indices, 6, 6);
}

int main() 
{
	Field eField("0", "0", "0", glm::vec3(0.0, 0.0, 0.0));
	Field bField("0", "0", "0", glm::vec3(0.0, 0.0, 0.0));
	Field gField("0", "0", "0", glm::vec3(0.0, 0.0, 0.0));

	std::string fromFile{ "" };

	std::cout << "\nSetup the fields with file? (Y/N)\n\n";
	std::cin >> fromFile;

	if (fromFile == "y" || fromFile == "Y") {
		std::string createSawtooth{ "" };

		std::cout << "\nCreate sawtooth magnetic field profile? (Y/N)\n\n";
		std::cin >> createSawtooth;

		if(createSawtooth == "y" || createSawtooth == "Y") {
			double magnitude{0.0};
			double period{0.0};
			size_t order{0};
			bool reverse{false};
			double offset{0.0};

			std::string tempBuffer{ "" };
			std::fstream file{"Initialization Data/sawtooth_config.txt", std::ios::in};
			
			if (file.is_open()) {
				std::getline(file, tempBuffer);
				tempBuffer.clear();
				std::getline(file, tempBuffer);
				magnitude = std::stof(tempBuffer);
				std::getline(file, tempBuffer);
				tempBuffer.clear();
				std::getline(file, tempBuffer);
				period = std::stof(tempBuffer);
				std::getline(file, tempBuffer);
				tempBuffer.clear();
				std::getline(file, tempBuffer);
				order = std::stoi(tempBuffer);
				std::getline(file, tempBuffer);
				tempBuffer.clear();
				std::getline(file, tempBuffer);
				if (tempBuffer == "true") {
					reverse = true;
				}
				else {
					reverse = false;
				}
				std::getline(file, tempBuffer);
				tempBuffer.clear();
				std::getline(file, tempBuffer);
				offset = std::stof(tempBuffer);
			}

			file.close();

			Sawtooth bWave{ magnitude, period, order, reverse, offset };

			std::ofstream write_bfile;

			write_bfile.open("Initialization Data/bField.txt");

			if (write_bfile.is_open()) {
				write_bfile << "x-component" << std::endl;
				write_bfile << bWave.getWave() << "+t" << std::endl;
				write_bfile << "y-component" << std::endl;
				write_bfile << "0" << std::endl;
				write_bfile << "z-component" << std::endl;
				write_bfile << "0" << std::endl;
			}

			write_bfile.close();

			std::ofstream write_efile;

			write_efile.open("Initialization Data/eField.txt");

			if (write_efile.is_open()) {
				write_efile << "x-component" << std::endl;
				write_efile << "-x" << std::endl;
				write_efile << "y-component" << std::endl;
				write_efile << "0.5*z*(" << bWave.getDer() << "+1" << ")" << std::endl;
				write_efile << "z-component" << std::endl;
				write_efile << "-0.5*y*(" << bWave.getDer() << "+1" << ")" << std::endl;
			}

			write_efile.close();
		}

		eField.setupField("electric", true, "Initialization Data/eField.txt");
		bField.setupField("magnetic", true, "Initialization Data/bField.txt");
		gField.setupField("gravitational", true, "Initialization Data/gField.txt");
	}
	else {
		std::cout << "\nFollow the instruction below and setup the electric field:\n\n";
		eField.setupField("electric", false, "");
		std::cout << "\nFollow the instruction below and setup the magnetic field:\n\n";
		bField.setupField("magnetic", false, "");
		std::cout << "\nFollow the instruction below and setup the gravitational field:\n\n";
		gField.setupField("gravitational", false, "");
	}

	std::vector <Particle> particleList;
	size_t numParticles{ 0 };

	try {
		ParticleGenerator(particleList);
	}
	catch (char* error) {
		std::cout << error;
	}

	std::cout << "========================================\n";
	std::cout << "Camera Control Instructions:\n\n";
	std::cout << "* Press \'W\' key to move forward.\n";
	std::cout << "* Press \'S\' key to move backward.\n";
	std::cout << "* Press \'A\' key to move to the left.\n";
	std::cout << "* Press \'D\' key to move to the right.\n";
	std::cout << "* Press \'E\' key to raise the camera.\n";
	std::cout << "* Press \'Q\' key to lower the camera.\n";
	std::cout << "* Press \'R\' key to increase the camera move speed.\n";
	std::cout << "* Press \'F\' key to decrease the camera move speed.\n";
	std::cout << "* Press \'escape\' key to terminate the program\n";
	std::cout << "========================================\n";


	std::cout << "========================================\n";
	std::cout << "\nScreen Resolution Setup\n";
	GLuint sWidth{0};
	GLuint sHeight{0};

	std::cout << "\nEnter the screen horizontal resolution:\n";
	std::cin >> sWidth;
	std::cout << "\nEnter the screen verticle resolution:\n";
	std::cin >> sHeight;

	mainWindow = Window(sWidth, sHeight);
	mainWindow.Initialise();

	numParticles = particleList.size();

	GLfloat* vertices = new GLfloat[numParticles * 3];
	unsigned int* indices = new unsigned int[numParticles];

	for (size_t i = 0; i < numParticles; i++) {
		indices[i] = i;
	}

	particlesToVertices(particleList, vertices);

	Mesh* particleGroup = new Mesh;
	particleGroup->CreateMesh(vertices, indices, numParticles, numParticles);
	
	Mesh* coordinateAxis = new Mesh;
	createAxis(coordinateAxis);

	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.0f, 0.1f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	double currentTime{0.0};

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;
		currentTime += deltaTime;

		for (auto i = particleList.begin(); i != particleList.end(); i++) {
			(*i).update(currentTime, deltaTime, eField, bField, gField);
		}

		particlesToVertices(particleList, vertices);

		particleGroup->UpdateMesh(vertices);

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//camera.scrollControl(mainWindow.getScrollXChange(), mainWindow.getScrollYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].GetColorLocation();

		glm::mat4 model(1.0f);
		glm::vec4 particleColor(1.0, 1.0, 0.5, 0.5);
		glm::vec4 axisColor(0.5, 0.5, 1.0, 1.0);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		
		glUniform4fv(uniformColor,1, glm::value_ptr(particleColor));
		particleGroup->RenderMeshParticle();
		glUniform4fv(uniformColor, 1, glm::value_ptr(axisColor));
		coordinateAxis->RenderMeshLine();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	delete[] vertices;
	delete[] indices;
	delete particleGroup;
	delete coordinateAxis;

	return 0;
}