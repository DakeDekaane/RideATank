//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>
#include <cstdlib>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/InputManager.h"
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"

// Texture include
#include "Headers/Texture.h"

// Camera include
//#include "Headers/CameraFPS.h"

#include "Headers/Model.h"

// Sphere include
#include "Headers/Sphere.h"

//Cubemap
#include "Headers/CubemapTexture.h"

//Collisions
//#include "Headers/collision.h"

// OpenAL include
#include <AL/alut.h>

Shader lightingShader;
Shader lampShader;
Shader cubemapShader;
Shader envCubeShader;
Shader bulletParticlesShader;
Shader explosionParticlesShader;
Shader smokeShader;

Model floor_model;
Model player_model;
Model enemy_model;
Model building1_model;
Model building2_model;
Model building3_model;
Model bullet_model;
Model lamp_base_model;

Texture textureParticle(GL_TEXTURE_2D, "../Textures/explosion.png");
Texture textureSmoke(GL_TEXTURE_2D, "../Textures/smoke.png");
CubemapTexture* cubeMapTexture = new CubemapTexture("../Textures", "sky.png", "sky.png", "sky.png", "sky.png", "sky.png", "sky.png");
CubemapTexture* cubeMapTextureNight = new CubemapTexture("../Textures", "sky-night.png", "sky-night.png", "sky-night.png", "sky-night.png", "sky-night.png", "sky-night.png");

Sphere sp(1.5, 20, 20, MODEL_MODE::VERTEX_COLOR);
Sphere sp2(1.5, 20, 20, MODEL_MODE::VERTEX_LIGHT_COLOR);

GLuint VAO, VBO, EBO;

const int BUILDING_PER_COLOR = 8;

int screenWidth;
int screenHeight;

GLFWwindow * window;
InputManager inputManager;
double deltaTime;

bool shot = false;
bool bullet_collision = false;
bool bullet_collision_enemy = false;

const int ENEMIES = 10;
bool enemy_alive[ENEMIES] = { true, true, true, true, true, true, true, true, true, true, };

glm::vec3 enemy_positions[] = {
	{ 5.0f, 2.0f, 6.0f },
	{ 15.0f, 2.0f, 6.0f },
	{ 25.0f, 2.0f, 6.0f },
	{ 35.0f, 2.0f, 6.0f },
	{ 5.0f, 2.0f, 16.0f },
	{ 5.0f, 2.0f, 26.0f },
	{ 5.0f, 2.0f, 36.0f },
	{ 15.0f, 2.0f, 16.0f },
	{ 25.0f, 2.0f, 26.0f },
	{ 35.0f, 2.0f, 36.0f },
};

Position player_pos = { 0.0f, 2.0f, 0.0f };

glm::vec3 skyCameraPos;
glm::vec3 skyCameraPosOffset = { 10.0f,20.0f,10.0f };
glm::vec3 skyCameraFront;
glm::vec3 skyCameraUp;

glm::vec3 bulletOffset = { 0.0f, 1.0f, 1.7f };

glm::vec3 building1_positions[] = {
	{ -40.0f, 0.0f, -40.0f },
	{ -12.0f, 0.0f, -40.0f },

	{ -40.0f, 0.0f, -32.0f },
	{ -12.0f, 0.0f, -32.0f },
	
	{ -32.0f, 0.0f, -40.0f },
	{ -4.0f, 0.0f, -40.0f },
	
	{ -32.0f, 0.0f, -32.0f },
	{ -4.0f, 0.0f, -32.0f }
};

glm::vec3 building2_positions[] = {
	{ -40.0f, 0.0f, -12.0f },
	{ -12.0f, 0.0f, -12.0f },
	
	{ -40.0f, 0.0f, -4.0f },
	{ -12.0f, 0.0f, -4.0f },
	
	{ -32.0f, 0.0f, -12.0f },
	{ -4.0f, 0.0f, -12.0f },

	{ -32.0f, 0.0f, -4.0f },
	{ -4.0f, 0.0f, -4.0f },
};

glm::vec3 building3_positions[] = {
	{ -40.0f, 0.0f, 16.0f },
	{ -12.0f, 0.0f, 16.0f },

	{ -40.0f, 0.0f, 24.0f },
	{ -12.0f, 0.0f, 24.0f },

	{ -32.0f, 0.0f, 16.0f },
	{ -4.0f, 0.0f, 16.0f },

	{ -32.0f, 0.0f, 24.0f },
	{ -4.0f, 0.0f, 24.0f },
};

//Point Lights
const int NUM_POINT_LIGHTS = 24;
glm::vec3 pointLightPositions[] = {
	glm::vec3(-44.5f, 3.0f, -42.5f),
	glm::vec3(-44.5f, 3.0f, -27.25f),
	glm::vec3(-44.5f, 3.0f, -14.25f),
	glm::vec3(-44.5f, 3.0f, 0.75f),
	glm::vec3(-44.5f, 3.0f, 13.5f),
	glm::vec3(-44.5f, 3.0f, 28.75f),

	glm::vec3(-29.25f, 3.0f, -42.5f),
	glm::vec3(-29.25f, 3.0f, -27.25f),
	glm::vec3(-29.25f, 3.0f, -14.25f),
	glm::vec3(-29.25f, 3.0f, 0.75f),
	glm::vec3(-29.25f, 3.0f, 13.5f),
	glm::vec3(-29.25f, 3.0f, 28.75f),

	glm::vec3(-16.5f, 3.0f, -42.5f),
	glm::vec3(-16.5f, 3.0f, -27.25f),
	glm::vec3(-16.5f, 3.0f, -14.25f),
	glm::vec3(-16.5f, 3.0f, 0.75f),
	glm::vec3(-16.5f, 3.0f, 13.5f),
	glm::vec3(-16.5f, 3.0f, 28.75f),

	glm::vec3(-1.25f, 3.0f, -42.5f),
	glm::vec3(-1.25f, 3.0f, -27.25f),
	glm::vec3(-1.25f, 3.0f, -14.25f),
	glm::vec3(-1.25f, 3.0f, 0.75f),
	glm::vec3(-1.25f, 3.0f, 13.5f),
	glm::vec3(-1.25f, 3.0f, 28.75f),


};

//Spotlights
const int NUM_SPOTLIGHTS = 2;
glm::vec3 tank_light_positions[] = {
	{ -0.6f,-0.04f,1.4f },
	{ 0.6f,-0.04f,1.4f }
};


//Colliders
AABB aabb_bullet_test;
AABB aabb1_test;
AABB aabb2_test[ENEMIES];
AABB aabb_building1[BUILDING_PER_COLOR];
AABB aabb_building2[BUILDING_PER_COLOR];
AABB aabb_building3[BUILDING_PER_COLOR];
AABB bounds[] = {
	{ glm::vec3(-50.0f,0.0f,-120.0f) , glm::vec3(50.0f,20.0f,-50.0f) },
	{ glm::vec3(-50.0f,0.0f,50.0f) , glm::vec3(50.0f,20.0f,120.0f) },
	{ glm::vec3(-120.0f,0.0f,-50.0f) , glm::vec3(-50.0f,20.0f,50.0f) },
	{ glm::vec3(50.0f,0.0f,-50.0f) , glm::vec3(120.0f,20.0f,50.0f) },
	{ glm::vec3(-50.0f,-120.0f,-50.0f) , glm::vec3(50.0f,0.0f,50.0f) },
	{ glm::vec3(-50.0f,20.0f,-50.0f) , glm::vec3(50.0f,120.0f,50.0f) },
};

// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles;

double particleStartTime;
double particleTime;
double explosionStartTime;
double explosionTime;
double smokeStartTime;
double smokeTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);

void initParticleBuffers() {
	nParticles = 500;

	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

								 // Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.5f, 1.5f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroyWindow();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable test depth, must be clear depth buffer bit
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	sp.init();
	sp.load();

	sp2.init();
	sp2.load();

	floor_model.loadModel("../models/buildings/floor.obj");
	player_model.loadModel("../models/tanks/tank-green.obj");
	enemy_model.loadModel("../models/tanks/tank-red.obj");
	bullet_model.loadModel("../models/tanks/bullet.obj");
	building1_model.loadModel("../models/buildings/building1.obj");
	building2_model.loadModel("../models/buildings/building2.obj");
	building3_model.loadModel("../models/buildings/building3.obj");
	lamp_base_model.loadModel("../models/buildings/lamp.obj");
	

	lightingShader.initialize("../Shaders/loadModelLighting.vs", "../Shaders/loadModelLighting.fs");
	lampShader.initialize("../Shaders/lampShader.vs", "../Shaders/lampShader.fs");
	cubemapShader.initialize("../Shaders/cubemapTexture.vs", "../Shaders/cubemapTexture.fs");
	envCubeShader.initialize("../Shaders/envRefCubemapTexture.vs", "../Shaders/envRefCubemapTexture.fs");
	bulletParticlesShader.initialize("../Shaders/particles.vs", "../Shaders/particles.fs");
	explosionParticlesShader.initialize("../Shaders/particles.vs", "../Shaders/particles.fs");
	smokeShader.initialize("../Shaders/particles.vs", "../Shaders/particles.fs");

	// The particle texture
	textureParticle.load();
	initParticleBuffers();
	cubeMapTexture->Load();
	cubeMapTextureNight->Load();

}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();
	lightingShader.destroy();
	//lampShader.destroy();
	cubemapShader.destroy();
	envCubeShader.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !shot) {
		smokeStartTime = TimeManager::Instance().GetTime();
		shot = true;
	}
	inputManager.keyPressed(inputManager.toApplicationKey(key), deltaTime * 10.0f,
		inputManager.toApplicationState(action));
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	inputManager.mouseMoved(xpos, ypos);
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	inputManager.mouseClicked(inputManager.toMouseButtonIndex(button), xpos,
		ypos, inputManager.toApplicationState(state));
}

bool processInput(bool continueApplication) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
		|| glfwWindowShouldClose(window) != 0) {
		return false;
	}
	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;
	inputManager.do_movement(deltaTime);
	inputManager.swapCamera();
	inputManager.changeTankLights();
	inputManager.changeDay();
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	//glm::vec3 lightPos(0.0f, 20.0f, 0.0f);
	glm::vec3 lightDir(0.2f, -1.0f, 0.2f);
	AABB aabb1 = getAABB(player_model.getMeshes());
	AABB aabb2 = getAABB(enemy_model.getMeshes());

	AABB aabb_b1 = getAABB(building1_model.getMeshes());
	AABB aabb_b2 = getAABB(building2_model.getMeshes());
	AABB aabb_b3 = getAABB(building3_model.getMeshes());

	AABB aabb_bullet = getAABB(bullet_model.getMeshes());

	double lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		psi = processInput(true);
		// This is new, need clear depth buffer bit
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.turnOn();

		GLint viewPosLoc = lightingShader.getUniformLocation("viewPos");
		if (inputManager.getActiveCamera() == PLAYER) {
			glUniform3f(viewPosLoc, inputManager.getCameraFPS()->Position.x, inputManager.getCameraFPS()->Position.y,
				inputManager.getCameraFPS()->Position.z);
		}
		else if (inputManager.getActiveCamera() == SKY) {
			glUniform3f(viewPosLoc, skyCameraPos.x, skyCameraPos.y, skyCameraPos.z);
		}


		// Set material properties
		GLint matDiffuseLoc = lightingShader.getUniformLocation("materialDiff");
		GLint matSpecularLoc = lightingShader.getUniformLocation("materialSpec");
		GLint matShineLoc = lightingShader.getUniformLocation("materialShine");
		glUniform1i(matDiffuseLoc, 0);
		glUniform1i(matSpecularLoc, 1);
		glUniform1f(matShineLoc, 32.0f);

		// Set lights properties
		//Directional light
		GLint lightAmbientLoc = lightingShader.getUniformLocation("dirLight.ambient");
		GLint lightDiffuseLoc = lightingShader.getUniformLocation("dirLight.diffuse");
		GLint lightSpecularLoc = lightingShader.getUniformLocation("dirLight.specular");
		GLint lightDirectionLoc = lightingShader.getUniformLocation("dirLight.direction");

		//Point Lights
		GLint lightAmbientLocP[NUM_POINT_LIGHTS];
		GLint lightDiffuseLocP[NUM_POINT_LIGHTS];
		GLint lightSpecularLocP[NUM_POINT_LIGHTS];
		GLint lightPositionLocP[NUM_POINT_LIGHTS];
		GLint lightConstantLocP[NUM_POINT_LIGHTS];
		GLint lightLinearLocP[NUM_POINT_LIGHTS];
		GLint lightQuadraticLocP[NUM_POINT_LIGHTS];
		for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
			lightAmbientLocP[i] = lightingShader.getUniformLocation("pointLights[" + std::to_string(i) + "].ambient");
			lightDiffuseLocP[i] = lightingShader.getUniformLocation("pointLights[" + std::to_string(i) + "].diffuse");
			lightSpecularLocP[i] = lightingShader.getUniformLocation("pointLights[" + std::to_string(i) + "].specular");
			lightPositionLocP[i] = lightingShader.getUniformLocation("pointLights[" + std::to_string(i) + "].position");
			lightConstantLocP[i] = lightingShader.getUniformLocation("pointLights[" + std::to_string(i) + "].constant");
			lightLinearLocP[i] = lightingShader.getUniformLocation("pointLights[" + std::to_string(i) + "].linear");
			lightQuadraticLocP[i] = lightingShader.getUniformLocation("pointLights[" + std::to_string(i) + "].quadratic");

		}

		//Tank spotlights
		GLint lightAmbientLocS[NUM_SPOTLIGHTS];
		GLint lightDiffuseLocS[NUM_SPOTLIGHTS];
		GLint lightSpecularLocS[NUM_SPOTLIGHTS];
		GLint lightPositionLocS[NUM_SPOTLIGHTS];
		GLint lightDirectionLocS[NUM_SPOTLIGHTS];
		GLint lightCutOff[NUM_SPOTLIGHTS];
		GLint lightOuterCutOff[NUM_SPOTLIGHTS];
		GLint lightConstantLocS[NUM_SPOTLIGHTS];
		GLint lightLinearLocS[NUM_SPOTLIGHTS];
		GLint lightQuadraticLocS[NUM_SPOTLIGHTS];
		glm::mat4 spot_mat[NUM_SPOTLIGHTS];
		glm::vec3 spot_pos[NUM_SPOTLIGHTS];
		for (int i = 0; i < NUM_SPOTLIGHTS; i++) {
			lightAmbientLocS[i] = lightingShader.getUniformLocation("spotLights[" + std::to_string(i) + "].ambient");
			lightDiffuseLocS[i] = lightingShader.getUniformLocation("spotLights[" + std::to_string(i) + "].diffuse");
			lightSpecularLocS[i] = lightingShader.getUniformLocation("spotLights[" + std::to_string(i) + "].specular");
			lightPositionLocS[i] = lightingShader.getUniformLocation("spotLights[" + std::to_string(i) + "].position");
			lightDirectionLocS[i] = lightingShader.getUniformLocation("spotLights[" + std::to_string(i) + "].direction");
			lightCutOff[i] = lightingShader.getUniformLocation("spotLights[" + std::to_string(i) + "].cutOff");
			lightOuterCutOff[i] = lightingShader.getUniformLocation("spotLights[" + std::to_string(i) + "].outerCutOff");
			lightConstantLocS[i] = lightingShader.getUniformLocation("spotLights[" + std::to_string(i) + "].constant");
			lightLinearLocS[i] = lightingShader.getUniformLocation("spotLights[" + std::to_string(i) + "].linear");
			lightQuadraticLocS[i] = lightingShader.getUniformLocation("spotLights[" + std::to_string(i) + "].quadratic");
			spot_mat[i] = glm::translate(glm::mat4(1.0f), inputManager.getCameraFPS()->Position);
			spot_mat[i] = glm::rotate(spot_mat[i], glm::radians(-inputManager.getCameraFPS()->Yaw + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			spot_mat[i] = glm::rotate(spot_mat[i], glm::radians(-inputManager.getCameraFPS()->Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
			spot_mat[i] = glm::translate(spot_mat[i], tank_light_positions[i]);
			spot_pos[i] = glm::vec3(spot_mat[i] * glm::vec4(0, 0, 0, 1));
		}

		

		if (inputManager.isDay()) {
			glUniform3f(lightAmbientLoc, 0.5f, 0.5f, 0.5f);
			glUniform3f(lightDiffuseLoc, 0.8f, 0.8f, 0.8f);
			glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
			for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
				glUniform3f(lightAmbientLocP[i], 0.0f, 0.0f, 0.0f);
				glUniform3f(lightDiffuseLocP[i], 0.0f, 0.0f, 0.0f);
				glUniform3f(lightSpecularLocP[i], 1.0f, 1.0f, 1.0f);
				glUniform3f(lightPositionLocP[i], pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
				glUniform1f(lightConstantLocP[i], 1.0f);
				glUniform1f(lightLinearLocP[i], 0.35f);
				glUniform1f(lightQuadraticLocP[i], 0.44f);
			}
		}
		else {
			glUniform3f(lightAmbientLoc, 0.05f, 0.05f, 0.05f);
			glUniform3f(lightDiffuseLoc, 0.1f, 0.1f, 0.1f);
			glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
			for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
				glUniform3f(lightAmbientLocP[i], 0.0f, 0.0f, 0.0f);
				glUniform3f(lightDiffuseLocP[i], 1.0f, 1.0f, 1.0f);
				glUniform3f(lightSpecularLocP[i], 1.0f, 1.0f, 1.0f);
				glUniform3f(lightPositionLocP[i], pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
				glUniform1f(lightConstantLocP[i], 1.0f);
				glUniform1f(lightLinearLocP[i], 0.22f);
				glUniform1f(lightQuadraticLocP[i], 0.20f);
			}
		}
		glUniform3f(lightDirectionLoc, lightDir.x, lightDir.y, lightDir.z);

		for (int i = 0; i < NUM_SPOTLIGHTS; i++) {
			if (inputManager.getTankLights()) {
				glUniform3f(lightAmbientLocS[i], 0.0f, 0.0f, 0.0f);
				glUniform3f(lightDiffuseLocS[i], 1.0f, 0.8f, 0.0f);
				glUniform3f(lightSpecularLocS[i], 1.0f, 1.0f, 1.0f);
			}
			else {
				glUniform3f(lightAmbientLocS[i], 0.0f, 0.0f, 0.0f);
				glUniform3f(lightDiffuseLocS[i], 0.0f, 0.0f, 0.0f);
				glUniform3f(lightSpecularLocS[i], 1.0f, 1.0f, 1.0f);
			}
			glUniform3f(lightPositionLocS[i], spot_pos[i].x, spot_pos[i].y, spot_pos[i].z );
			glUniform3f(lightDirectionLocS[i], inputManager.getCameraFPS()->Front.x, inputManager.getCameraFPS()->Front.y, inputManager.getCameraFPS()->Front.z);
			glUniform1f(lightCutOff[i], glm::cos(glm::radians(12.5f)));
			glUniform1f(lightOuterCutOff[i], glm::cos(glm::radians(17.5f)));
			glUniform1f(lightConstantLocS[i], 1.0f);
			glUniform1f(lightLinearLocS[i], 0.14f);
			glUniform1f(lightQuadraticLocS[i], 0.07f);
		}

		// Create camera transformations
		glm::mat4 view = glm::mat4(1.0f);
		if (inputManager.getActiveCamera() == PLAYER) {
			view = inputManager.getCameraFPS()->GetViewMatrix();
		}
		else if (inputManager.getActiveCamera() == SKY) {
			inputManager.getCameraFPS()->Pitch = 0.0f;
			skyCameraPos = inputManager.getCameraFPS()->Position + skyCameraPosOffset;
			skyCameraFront = skyCameraPos - skyCameraPosOffset;
			skyCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			//skyCameraUp = glm::cross(glm::cross(skyCameraPos, skyCameraPosOffset),-skyCameraFront);
			view = glm::lookAt(skyCameraPos, skyCameraFront, skyCameraUp);
		}
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = lightingShader.getUniformLocation("model");
		GLint viewLoc = lightingShader.getUniformLocation("view");
		GLint projLoc = lightingShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Colliders


		// Draw the floor model
		glm::mat4 model0 = glm::mat4(1.0f);
		model0 = glm::translate(model0, glm::vec3(0.0f, 0.0f, 0.0f));
		model0 = glm::scale(model0, glm::vec3(50.0f, 0.01f, 50.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model0));
		floor_model.render(&lightingShader);

		// Draw the player model
		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, inputManager.getCameraFPS()->Position);
		model1 = glm::scale(model1, glm::vec3(1.0f, 1.0f, 1.0f));
		model1 = glm::rotate(model1, glm::radians(-inputManager.getCameraFPS()->Yaw + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		player_model.render(&lightingShader);
		aabb1_test.max = aabb1.max + glm::vec3(model1 *  glm::vec4(0, 0, 0, 1));
		aabb1_test.min = aabb1.min + glm::vec3(model1 *  glm::vec4(0, 0, 0, 1));

		// Draw the enemy model
		glm::mat4 model2[ENEMIES];
		for (int i = 0; i < ENEMIES; i++) {
			if (enemy_alive[i]) {
				model2[i] = glm::mat4(1.0f);
				model2[i] = glm::translate(model2[i], enemy_positions[i]);
				model2[i] = glm::scale(model2[i], glm::vec3(1.0f, 1.0f, 1.0f));
				model2[i] = glm::rotate(model2[i], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2[i]));
				enemy_model.render(&lightingShader);
				aabb2_test[i].max = aabb2.max + glm::vec3(model2[i] * glm::vec4(0, 0, 0, 1));
				aabb2_test[i].min = aabb2.min + glm::vec3(model2[i] * glm::vec4(0, 0, 0, 1));
			}
		}

		//Bullet
		glm::mat4 model_bullet = glm::mat4(1.0f);
		glm::vec3 bulletPosInit;
		float bulletAnglePitchInit;
		float bulletAngleYawInit;
		glm::vec3 hit_point;
		glm::vec3 explosion_origin;

		if (!shot) {
			bullet_collision = false;
			bullet_collision_enemy = false;
			lastTime = TimeManager::Instance().GetTime();
			bulletPosInit = inputManager.getCameraFPS()->Position;
			bulletAngleYawInit = -inputManager.getCameraFPS()->Yaw + 90.0f;
			bulletAnglePitchInit = -inputManager.getCameraFPS()->Pitch;
		}

		if (shot) {
			GLfloat bullet_speed = 0.1f;
			GLfloat position = 0.0;
			if (!bullet_collision) {
				GLfloat timeValue = TimeManager::Instance().GetTime() - lastTime;
				model_bullet = glm::translate(model_bullet, bulletPosInit);
				model_bullet = glm::rotate(model_bullet, glm::radians(bulletAngleYawInit), glm::vec3(0.0f, 1.0f, 0.0f));
				model_bullet = glm::rotate(model_bullet, glm::radians(bulletAnglePitchInit), glm::vec3(1.0f, 0.0f, 0.0f));
				model_bullet = glm::translate(model_bullet, bulletOffset);
				model_bullet = glm::scale(model_bullet, glm::vec3(0.3f, 0.3f, 0.3f));
				model_bullet = glm::translate(model_bullet, glm::vec3(0.0f, 0.0f, timeValue * 15));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_bullet));
				bullet_model.render(&lightingShader);
				aabb_bullet_test.max = aabb_bullet.max * 0.3f + glm::vec3(model_bullet *  glm::vec4(0, 0, 0, 1));
				aabb_bullet_test.min = aabb_bullet.min * 0.3f + glm::vec3(model_bullet *  glm::vec4(0, 0, 0, 1));

				//Collision detection and resolution
				//Enemy collision
				for (int i = 0; i < ENEMIES; i++) {
					if (testBoxBoxIntersection(aabb_bullet_test, aabb2_test[i])) {
						//std::cout << "Bullet hit: Enemy" << std::endl;
						bullet_collision = true;
						shot = false;
						enemy_alive[i] = false;
						hit_point = aabb_bullet_test.getCenter();
						explosion_origin = aabb2_test[i].getCenter();
						aabb2_test[i].max = { 0.0f,-2.0f,0.0f };
						aabb2_test[i].min = { 0.0f,-2.0f,0.0f };
						particleStartTime = explosionStartTime = TimeManager::Instance().GetTime();

						//inputManager.setCollision(true, getCollisionDirection2D(aabb_bullet_test.getCenter(), aabb2_test.getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
					}
				}
				//Building collision
				for (int i = 0; i < BUILDING_PER_COLOR; i++) {
					if (testBoxBoxIntersection(aabb_bullet_test, aabb_building1[i])) {
						//std::cout << "Bullet hit: Orange Building." << std::endl;
						bullet_collision = true;
						shot = false;
						hit_point = aabb_bullet_test.getCenter();
						particleStartTime = TimeManager::Instance().GetTime();
						//inputManager.setCollision(true, getCollisionDirection2D(aabb_bullet_test.getCenter(), aabb_building1[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
					}
					if (testBoxBoxIntersection(aabb_bullet_test, aabb_building2[i])) {
						//std::cout << "Bullet hit: Green Building." << std::endl;
						bullet_collision = true;
						shot = false;
						hit_point = aabb_bullet_test.getCenter();
						particleStartTime = TimeManager::Instance().GetTime();
						//inputManager.setCollision(true, getCollisionDirection2D(aabb_bullet_test.getCenter(), aabb_building2[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
					}
					if (testBoxBoxIntersection(aabb_bullet_test, aabb_building3[i])) {
						//std::cout << "Bullet hit: Blue Building." << std::endl;
						bullet_collision = true;
						shot = false;
						hit_point = aabb_bullet_test.getCenter();
						particleStartTime = TimeManager::Instance().GetTime();
						//inputManager.setCollision(true, getCollisionDirection2D(aabb_bullet_test.getCenter(), aabb_building3[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
					}
				}
				//Bounds collision
				for (int i = 0; i < 6; i++) {
					if (testBoxBoxIntersection(aabb_bullet_test, bounds[i])) {
						//std::cout << "Bullet hit: Bounds" << std::endl;
						bullet_collision = true;
						shot = false;
						hit_point = aabb_bullet_test.getCenter();
						particleStartTime = TimeManager::Instance().GetTime();
						//inputManager.setCollision(true, getCollisionDirection2D(aabb_bullet_test.getCenter(), bounds[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
					}
				}

			}
		}


		// Draw the building model
		glm::mat4 model4[BUILDING_PER_COLOR] = { glm::mat4(1.0f) };
		for (int i = 0; i < BUILDING_PER_COLOR; i++) {
			model4[i] = glm::mat4(1.0f);
			model4[i] = glm::translate(model4[i], building1_positions[i]);
			model4[i] = glm::scale(model4[i], glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4[i]));
			building1_model.render(&lightingShader);
			aabb_building1[i].max = aabb_b1.max * 2.0f + glm::vec3(model4[i] * glm::vec4(0, 0, 0, 1));
			aabb_building1[i].min = aabb_b1.min * 2.0f + glm::vec3(model4[i] * glm::vec4(0, 0, 0, 1));
		}

		// Draw the building model
		glm::mat4 model5[i] = { glm::mat4(1.0f) };
		for (int i = 0; i < BUILDING_PER_COLOR; i++) {
			model5[i] = glm::mat4(1.0f);
			model5[i] = glm::translate(model5[i], building2_positions[i]);
			model5[i] = glm::scale(model5[i], glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5[i]));
			building2_model.render(&lightingShader);
			aabb_building2[i].max = aabb_b2.max * 2.0f + glm::vec3(model5[i] * glm::vec4(0, 0, 0, 1));
			aabb_building2[i].min = aabb_b2.min * 2.0f + glm::vec3(model5[i] * glm::vec4(0, 0, 0, 1));
		}

		// Draw the building model
		glm::mat4 model6[i] = { glm::mat4(1.0f) };
		for (int i = 0; i < BUILDING_PER_COLOR; i++) {
			model6[i] = glm::mat4(1.0f);
			model6[i] = glm::translate(model6[i], building3_positions[i]);
			model6[i] = glm::scale(model6[i], glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6[i]));
			building3_model.render(&lightingShader);
			aabb_building3[i].max = aabb_b2.max * 2.0f + glm::vec3(model6[i] * glm::vec4(0, 0, 0, 1));
			aabb_building3[i].min = aabb_b2.min * 2.0f + glm::vec3(model6[i] * glm::vec4(0, 0, 0, 1));
		}

		//Draw lamp bases
		glm::mat4 model7;
		for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
			model7 = glm::mat4(1.0f);
			model7 = glm::translate(model7, pointLightPositions[i] - glm::vec3(0.0f, 2.75f, 0.0f));
			model7 = glm::scale(model7, glm::vec3(1.0f, 0.80f, 1.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model7));
			lamp_base_model.render(&lightingShader);
			
		}

		lightingShader.turnOff();

		//Smoke 
		smokeShader.turnOn();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Set the point size
		if (inputManager.getActiveCamera() == PLAYER) {
			glPointSize(30.0f);
		}
		if (inputManager.getActiveCamera() == SKY) {
			glPointSize(10.0f);
		}

		textureSmoke.bind(GL_TEXTURE0);

		modelLoc = smokeShader.getUniformLocation("model");
		viewLoc = smokeShader.getUniformLocation("view");
		projLoc = smokeShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		smokeTime = TimeManager::Instance().GetTime() - smokeStartTime;
		glUniform1f(smokeShader.getUniformLocation("Time"), float(smokeTime) * 2);
		glUniform1f(smokeShader.getUniformLocation("ParticleTex"), 0);
		glUniform1f(smokeShader.getUniformLocation("ParticleLifetime"), 1.5f);
		glUniform3fv(smokeShader.getUniformLocation("Gravity"), 1,
			glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.5f)));

		glm::mat4 model_smoke = glm::mat4(1.0f);
		model_smoke = glm::translate(model_smoke, bulletPosInit);
		model_smoke = glm::rotate(model_smoke, glm::radians(bulletAngleYawInit), glm::vec3(0.0f, 1.0f, 0.0f));
		model_smoke = glm::rotate(model_smoke, glm::radians(bulletAnglePitchInit), glm::vec3(1.0f, 0.0f, 0.0f));
		model_smoke = glm::translate(model_smoke, bulletOffset);
		model_smoke = glm::rotate(model_smoke, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_smoke));

		glBindVertexArray(VAOParticles);
		glDrawArrays(GL_POINTS, 0, nParticles);
		glDisable(GL_BLEND);

		smokeShader.turnOff();

		//Bullet particles
		bulletParticlesShader.turnOn();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Set the point size
		if (inputManager.getActiveCamera() == PLAYER) {
			glPointSize(30.0f);
		}
		if (inputManager.getActiveCamera() == SKY) {
			glPointSize(10.0f);
		}

		textureParticle.bind(GL_TEXTURE0);

		modelLoc = bulletParticlesShader.getUniformLocation("model");
		viewLoc = bulletParticlesShader.getUniformLocation("view");
		projLoc = bulletParticlesShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		particleTime = TimeManager::Instance().GetTime() - particleStartTime;
		glUniform1f(bulletParticlesShader.getUniformLocation("Time"), float(particleTime) * 2);
		glUniform1f(bulletParticlesShader.getUniformLocation("ParticleTex"), 0);
		glUniform1f(bulletParticlesShader.getUniformLocation("ParticleLifetime"), 3.5f);
		glUniform3fv(bulletParticlesShader.getUniformLocation("Gravity"), 1,
			glm::value_ptr(glm::vec3(0.0f, -1.0f, 0.0f)));

		glm::mat4 model_particle = glm::mat4(1.0f);
		model_particle = glm::translate(model_particle, hit_point);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_particle));

		glBindVertexArray(VAOParticles);
		glDrawArrays(GL_POINTS, 0, nParticles);
		glDisable(GL_BLEND);

		bulletParticlesShader.turnOff();

		//Destroyed enemy particles
		explosionParticlesShader.turnOn();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Set the point size
		if (inputManager.getActiveCamera() == PLAYER) {
			glPointSize(50.0f);
		}
		if (inputManager.getActiveCamera() == SKY) {
			glPointSize(30.0f);
		}

		textureParticle.bind(GL_TEXTURE0);

		modelLoc = explosionParticlesShader.getUniformLocation("model");
		viewLoc = explosionParticlesShader.getUniformLocation("view");
		projLoc = explosionParticlesShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		explosionTime = TimeManager::Instance().GetTime() - explosionStartTime;
		glUniform1f(explosionParticlesShader.getUniformLocation("Time"), float(explosionTime) * 2);
		glUniform1f(explosionParticlesShader.getUniformLocation("ParticleTex"), 0);
		glUniform1f(explosionParticlesShader.getUniformLocation("ParticleLifetime"), 4.0f);
		glUniform3fv(explosionParticlesShader.getUniformLocation("Gravity"), 1,
			glm::value_ptr(glm::vec3(0.0f, -1.0f, 0.0f)));

		glm::mat4 explosion_model = glm::mat4(1.0f);
		explosion_model = glm::translate(explosion_model, explosion_origin);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(explosion_model));

		glBindVertexArray(VAOParticles);
		glDrawArrays(GL_POINTS, 0, nParticles);
		glDisable(GL_BLEND);

		explosionParticlesShader.turnOff();

		lampShader.turnOn();
		// Create transformations
		modelLoc = lampShader.getUniformLocation("model");
		viewLoc = lampShader.getUniformLocation("view");
		projLoc = lampShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model_lamp;
		for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
			model_lamp = glm::translate(glm::mat4(1.0f), pointLightPositions[i]);
			model_lamp = glm::scale(model_lamp, glm::vec3(0.27, 0.27, 0.27));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_lamp));
			sp.render();
		}
		for (int i = 0; i < NUM_SPOTLIGHTS; i++){
			model_lamp = glm::translate(glm::mat4(1.0f), inputManager.getCameraFPS()->Position);
			model_lamp = glm::rotate(model_lamp, glm::radians(-inputManager.getCameraFPS()->Yaw + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model_lamp = glm::translate(model_lamp, tank_light_positions[i]);
			model_lamp = glm::scale(model_lamp, glm::vec3(0.1, 0.1, 0.1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_lamp));
			sp.render();
		}
		lampShader.turnOff();

		cubemapShader.turnOn();

		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;

		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

		GLint viewLoc2 = cubemapShader.getUniformLocation("view");
		GLint projLoc2 = cubemapShader.getUniformLocation("projection");
		GLint modelLoc2 = cubemapShader.getUniformLocation("model");

		view = glm::mat3(inputManager.getCameraFPS()->GetViewMatrix());
		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 cubeModel = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(cubeModel));

		if (inputManager.isDay()) {
			cubeMapTexture->Bind(GL_TEXTURE0);
		}
		
		else {
			cubeMapTextureNight->Bind(GL_TEXTURE0);
		}
		GLuint cubeTextureId = cubemapShader.getUniformLocation("skybox");
		glUniform1f(cubeTextureId, 0);

		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		sp2.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		cubemapShader.turnOff();

		//Collision detection and resolution
		//Enemy collision
		inputManager.setCollision(false);
		for (int i = 0; i < ENEMIES; i++) {
			if (testBoxBoxIntersection(aabb1_test, aabb2_test[i])) {
				//std::cout << "Model collision: Enemy" << std::endl;
				inputManager.setCollision(true, getCollisionDirection2D(aabb1_test.getCenter(), aabb2_test[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
			}
		}
		//Building collision
		for (int i = 0; i < BUILDING_PER_COLOR; i++) {
			if (testBoxBoxIntersection(aabb1_test, aabb_building1[i])) {
				//std::cout << "Model collision: Orange Building." << std::endl;
				inputManager.setCollision(true, getCollisionDirection2D(aabb1_test.getCenter(), aabb_building1[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
			}
			if (testBoxBoxIntersection(aabb1_test, aabb_building2[i])) {
				//std::cout << "Model collision: Green Building." << std::endl;
				inputManager.setCollision(true, getCollisionDirection2D(aabb1_test.getCenter(), aabb_building2[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
			}
			if (testBoxBoxIntersection(aabb1_test, aabb_building3[i])) {
				//std::cout << "Model collision: Blue Building." << std::endl;
				inputManager.setCollision(true, getCollisionDirection2D(aabb1_test.getCenter(), aabb_building3[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
			}
		}
		//Bounds collision
		for (int i = 0; i < 4; i++) {
			if (testBoxBoxIntersection(aabb1_test, bounds[i])) {
//				std::cout << "Model collision: Bounds" << std::endl;
				inputManager.setCollision(true, getCollisionDirection2D(aabb1_test.getCenter(), bounds[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
			}
		}

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
