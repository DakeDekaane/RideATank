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

Model floor_model;
Model player_model;
Model enemy_model;
Model building1_model;
Model building2_model;
Model building3_model;

CubemapTexture* cubeMaptexture = new CubemapTexture("../Textures", "sky.png", "sky.png", "sky.png", "sky.png", "sky.png", "sky.png");

Sphere sp(1.5, 50, 50, MODEL_MODE::VERTEX_COLOR);
Sphere sp2(1.5, 50, 50, MODEL_MODE::VERTEX_LIGHT_TEXTURE);

GLuint VAO, VBO, EBO;

int screenWidth;
int screenHeight;

GLFWwindow * window;
InputManager inputManager;
double deltaTime;

Position player_pos = { 0.0f, 2.0f, 0.0f };

glm::vec3 skyCameraPos = { 10.0f,20.0f,10.0f };
glm::vec3 skyCameraFront = { 0.0f,-1.0f,0.0f };
glm::vec3 skyCameraUp = { 1.0f,0.0f,0.0f };

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);

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
	building1_model.loadModel("../models/buildings/building1.obj");
	building2_model.loadModel("../models/buildings/building2.obj");
	building3_model.loadModel("../models/buildings/building3.obj");
	

	lightingShader.initialize("../Shaders/loadModelLighting.vs", "../Shaders/loadModelLightingDirectional.fs");
	//lampShader.initialize("../Shaders/lampShader.vs", "../Shaders/lampShader.fs");
	cubemapShader.initialize("../Shaders/cubemapTexture.vs", "../Shaders/cubemapTexture.fs");
	envCubeShader.initialize("../Shaders/envRefCubemapTexture.vs", "../Shaders/envRefCubemapTexture.fs");

	cubeMaptexture->Load();
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
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	//glm::vec3 lightPos(0.0f, 20.0f, 0.0f);
	glm::vec3 lightDir(0.2f, -1.0f, 0.2f);
	double lastTime = TimeManager::Instance().GetTime();

	AABB aabb1 = getAABB(player_model.getMeshes());
	AABB aabb2 = getAABB(enemy_model.getMeshes());

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
		GLint lightAmbientLoc = lightingShader.getUniformLocation("light.ambient");
		GLint lightDiffuseLoc = lightingShader.getUniformLocation("light.diffuse");
		GLint lightSpecularLoc = lightingShader.getUniformLocation("light.specular");
		//GLint lightPosLoc = lightingShader.getUniformLocation("light.position");
		GLint lightDirectionLoc = lightingShader.getUniformLocation("light.direction");
		glUniform3f(lightAmbientLoc, 0.5f, 0.5f, 0.5f);
		glUniform3f(lightDiffuseLoc, 0.8f, 0.8f, 0.8f); // Let's darken the light a bit to fit the scene
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(lightDirectionLoc, lightDir.x, lightDir.y, lightDir.z);

		// Create camera transformations
		glm::mat4 view = glm::mat4(1.0f);
		if (inputManager.getActiveCamera() == PLAYER) {
			view = inputManager.getCameraFPS()->GetViewMatrix();
		}
		else if (inputManager.getActiveCamera() == SKY) {
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

		GLfloat timeValue = TimeManager::Instance().GetTime() - lastTime;

		// Draw the floor model
		glm::mat4 model0 = glm::mat4(1.0f);
		model0 = glm::translate(model0, glm::vec3(0.0f, 0.0f, 0.0f));
		model0 = glm::scale(model0, glm::vec3(50.0f, 0.01f, 50.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model0));
		floor_model.render(&lightingShader);

		// Draw the player model
		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(inputManager.getCameraFPS()->Position.x, inputManager.getCameraFPS()->Position.y, inputManager.getCameraFPS()->Position.z));
		model1 = glm::scale(model1, glm::vec3(1.0f, 1.0f, 1.0f));
		model1 = glm::rotate(model1, glm::radians(-inputManager.getCameraFPS()->Yaw + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		player_model.render(&lightingShader);

		// Draw the enemy model
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(5.0f, 2.0f, 6.0f));
		model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));
		model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		enemy_model.render(&lightingShader);

		// Draw the building model
		glm::mat4 model4 = glm::mat4(1.0f);
		model4 = glm::translate(model4, glm::vec3(10.0f, 0.0f, -10.0f));
		model4 = glm::scale(model4, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
		building1_model.render(&lightingShader);

		// Draw the building model
		glm::mat4 model5 = glm::mat4(1.0f);
		model5 = glm::translate(model5, glm::vec3(10.0f, 0.0f, 0.0f));
		model5 = glm::scale(model5, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
		building2_model.render(&lightingShader);

		// Draw the building model
		glm::mat4 model6 = glm::mat4(1.0f);
		model6 = glm::translate(model6, glm::vec3(10.0f, 0.0f, 10.0f));
		model6 = glm::scale(model6, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6));
		building3_model.render(&lightingShader);
		
		lightingShader.turnOff();


		cubemapShader.turnOn();

		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;

		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

		viewLoc = cubemapShader.getUniformLocation("view");
		projLoc = cubemapShader.getUniformLocation("projection");
		modelLoc = cubemapShader.getUniformLocation("model");

		view = glm::mat3(inputManager.getCameraFPS()->GetViewMatrix());
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 cubeModel = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubeModel));

		cubeMaptexture->Bind(GL_TEXTURE0);
		GLuint cubeTextureId = cubemapShader.getUniformLocation("skybox");
		glUniform1f(cubeTextureId, 0);

		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		sp2.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		cubemapShader.turnOff();

		AABB aabb1_test;
		AABB aabb2_test;

		aabb1_test.max = aabb1.max + glm::vec3(model1 *  glm::vec4(0, 0, 0, 1));
		aabb1_test.min = aabb1.min + glm::vec3(model1 *  glm::vec4(0, 0, 0, 1));
		aabb2_test.max = aabb2.max + glm::vec3(model2 *  glm::vec4(0, 0, 0, 1));
		aabb2_test.min = aabb2.min + glm::vec3(model2 *  glm::vec4(0, 0, 0, 1));

		if (testBoxBoxIntersection(aabb1_test, aabb2_test)) {
			std::cout << "Model collision:" << std::endl;
			inputManager.setCollision(true,getCollisionDirection2D(glm::vec3(model1 *  glm::vec4(0, 0, 0, 1)), glm::vec3(model2 *  glm::vec4(0, 0, 0, 1)), -inputManager.getCameraFPS()->Yaw + 90.0f));
		}
		else {
			inputManager.setCollision(false);
		}
			

		/*lampShader.turnOn();
		// Create transformations
		modelLoc = lampShader.getUniformLocation("model");
		viewLoc = lampShader.getUniformLocation("view");
		projLoc = lampShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::translate(glm::mat4(1.0f), lightPos);
		model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		lampShader.turnOff();*/

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}

