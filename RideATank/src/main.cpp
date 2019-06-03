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
Model bullet_model;

CubemapTexture* cubeMaptexture = new CubemapTexture("../Textures", "sky.png", "sky.png", "sky.png", "sky.png", "sky.png", "sky.png");

Sphere sp(1.5, 50, 50, MODEL_MODE::VERTEX_COLOR);
Sphere sp2(1.5, 50, 50, MODEL_MODE::VERTEX_LIGHT_TEXTURE);

GLuint VAO, VBO, EBO;

const int BUILDING_PER_COLOR = 8;

//Colliders
AABB aabb_bullet_test;
AABB aabb1_test;
AABB aabb2_test;
AABB aabb_building1[BUILDING_PER_COLOR];
AABB aabb_building2[BUILDING_PER_COLOR];
AABB aabb_building3[BUILDING_PER_COLOR];

GLint modelLoc;


int screenWidth;
int screenHeight;

GLFWwindow * window;
InputManager inputManager;
double deltaTime;

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

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);
void shoot();

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
	//if (key == GLFW_KEY_SPACE || action == GLFW_PRESS) {
//		shoot();
	//}
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
	inputManager.changeDay();
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

	AABB aabb_b1 = getAABB(building1_model.getMeshes());
	AABB aabb_b2 = getAABB(building2_model.getMeshes());
	AABB aabb_b3 = getAABB(building3_model.getMeshes());

	AABB aabb_bullet = getAABB(bullet_model.getMeshes());

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

		if (inputManager.isDay()) {
			glUniform3f(lightAmbientLoc, 0.5f, 0.5f, 0.5f);
			glUniform3f(lightDiffuseLoc, 0.8f, 0.8f, 0.8f);
			glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
			glUniform3f(lightDiffuseLoc, 0.2f, 0.2f, 0.2f);
			glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		}
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(lightDirectionLoc, lightDir.x, lightDir.y, lightDir.z);

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
		modelLoc = lightingShader.getUniformLocation("model");
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
		model1 = glm::translate(model1, inputManager.getCameraFPS()->Position);
		model1 = glm::scale(model1, glm::vec3(1.0f, 1.0f, 1.0f));
		model1 = glm::rotate(model1, glm::radians(-inputManager.getCameraFPS()->Yaw + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		player_model.render(&lightingShader);
		aabb1_test.max = aabb1.max + glm::vec3(model1 *  glm::vec4(0, 0, 0, 1));
		aabb1_test.min = aabb1.min + glm::vec3(model1 *  glm::vec4(0, 0, 0, 1));

		// Draw the enemy model
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(5.0f, 2.0f, 6.0f));
		model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));
		model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		enemy_model.render(&lightingShader);
		aabb2_test.max = aabb2.max + glm::vec3(model2 *  glm::vec4(0, 0, 0, 1));
		aabb2_test.min = aabb2.min + glm::vec3(model2 *  glm::vec4(0, 0, 0, 1));

		//Bullet
		glm::mat4 model_bullet = glm::mat4(1.0f);
		
		

		// Draw the building model
		glm::mat4 model4[BUILDING_PER_COLOR] = { glm::mat4(1.0f) };
		for (int i = 0; i < BUILDING_PER_COLOR; i++) {
			model4[i] = glm::mat4(1.0f);
			model4[i] = glm::translate(model4[i], building1_positions[i]);
			model4[i] = glm::scale(model4[i], glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4[i]));
			building1_model.render(&lightingShader);
			aabb_building1[i].max = aabb_b1.max * 2.0f + glm::vec3(model4[i] *  glm::vec4(0, 0, 0, 1));
			aabb_building1[i].min = aabb_b1.min * 2.0f + glm::vec3(model4[i] *  glm::vec4(0, 0, 0, 1));
		}

		// Draw the building model
		glm::mat4 model5[i] = { glm::mat4(1.0f) };
		for (int i = 0; i < BUILDING_PER_COLOR; i++) {
			model5[i] = glm::mat4(1.0f);
			model5[i] = glm::translate(model5[i], building2_positions[i]);
			model5[i] = glm::scale(model5[i], glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5[i]));
			building2_model.render(&lightingShader);
			aabb_building2[i].max = aabb_b2.max * 2.0f + glm::vec3(model5[i] *  glm::vec4(0, 0, 0, 1));
			aabb_building2[i].min = aabb_b2.min * 2.0f + glm::vec3(model5[i] *  glm::vec4(0, 0, 0, 1));
		}

		// Draw the building model
		glm::mat4 model6[i] = { glm::mat4(1.0f) };
		for (int i = 0; i < BUILDING_PER_COLOR; i++) {
			model6[i] = glm::mat4(1.0f);
			model6[i] = glm::translate(model6[i], building3_positions[i]);
			model6[i] = glm::scale(model6[i], glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6[i]));
			building3_model.render(&lightingShader);
			aabb_building3[i].max = aabb_b2.max * 2.0f + glm::vec3(model6[i] *  glm::vec4(0, 0, 0, 1));
			aabb_building3[i].min = aabb_b2.min * 2.0f + glm::vec3(model6[i] *  glm::vec4(0, 0, 0, 1));
		}
		
		lightingShader.turnOff();
		
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

		cubeMaptexture->Bind(GL_TEXTURE0);
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
		if (testBoxBoxIntersection(aabb1_test, aabb2_test)) {
			std::cout << "Model collision: Enemy" << std::endl;
			inputManager.setCollision(true,getCollisionDirection2D(aabb1_test.getCenter(), aabb2_test.getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
		}
		else {
			inputManager.setCollision(false);
		}
		//Building collision
		for (int i = 0; i < BUILDING_PER_COLOR; i++) {
			if (testBoxBoxIntersection(aabb1_test, aabb_building1[i])) {
				std::cout << "Model collision: Orange Building." << std::endl;
				inputManager.setCollision(true, getCollisionDirection2D(aabb1_test.getCenter(), aabb_building1[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
			}
			if (testBoxBoxIntersection(aabb1_test, aabb_building2[i])) {
				std::cout << "Model collision: Green Building." << std::endl;
				inputManager.setCollision(true, getCollisionDirection2D(aabb1_test.getCenter(), aabb_building2[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
			}
			if (testBoxBoxIntersection(aabb1_test, aabb_building3[i])) {
				std::cout << "Model collision: Blue Building." << std::endl;
				inputManager.setCollision(true, getCollisionDirection2D(aabb1_test.getCenter(), aabb_building3[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
			}
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

void shoot() {
	AABB aabb = getAABB(bullet_model.getMeshes());
	glm::mat4 model_mat = glm::mat4(1.0f);
	model_mat = glm::translate(model_mat, inputManager.getCameraFPS()->Position);
	model_mat = glm::rotate(model_mat, glm::radians(-inputManager.getCameraFPS()->Yaw + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model_mat = glm::translate(model_mat, bulletOffset);
	model_mat = glm::scale(model_mat, glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_mat));
	bullet_model.render(&lightingShader);
	aabb_bullet_test.max = aabb.max * 0.3f + glm::vec3(model_mat *  glm::vec4(0, 0, 0, 1));
	aabb_bullet_test.min = aabb.min * 0.3f + glm::vec3(model_mat *  glm::vec4(0, 0, 0, 1));
	double lastTime = TimeManager::Instance().GetTime();
	bool collision = false;
	GLfloat bullet_speed = 0.1f;
	GLfloat position = 0.0;
	while (!collision) {
		GLfloat timeValue = TimeManager::Instance().GetTime() - lastTime;
		position += bullet_speed * timeValue;
		model_mat = glm::translate(model_mat, glm::vec3(0.0f, 0.0f, position));

		//Collision detection and resolution
		//Enemy collision
		if (testBoxBoxIntersection(aabb1_test, aabb2_test)) {
			std::cout << "Bullet hit: Enemy" << std::endl;
			collision = true;
			//inputManager.setCollision(true, getCollisionDirection2D(aabb_bullet_test.getCenter(), aabb2_test.getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
		}
		else {
			inputManager.setCollision(false);
		}
		//Building collision
		for (int i = 0; i < BUILDING_PER_COLOR; i++) {
			if (testBoxBoxIntersection(aabb1_test, aabb_building1[i])) {
				std::cout << "Bullet hit: Orange Building." << std::endl;
				collision = true;
				//inputManager.setCollision(true, getCollisionDirection2D(aabb_bullet_test.getCenter(), aabb_building1[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
			}
			if (testBoxBoxIntersection(aabb1_test, aabb_building2[i])) {
				std::cout << "Bullet hit: Green Building." << std::endl;
				collision = true;
				//inputManager.setCollision(true, getCollisionDirection2D(aabb_bullet_test.getCenter(), aabb_building2[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
			}
			if (testBoxBoxIntersection(aabb1_test, aabb_building3[i])) {
				std::cout << "Bullet hit: Blue Building." << std::endl;
				collision = true;
				//inputManager.setCollision(true, getCollisionDirection2D(aabb_bullet_test.getCenter(), aabb_building3[i].getCenter(), -inputManager.getCameraFPS()->Yaw + 90.0f));
			}
		}

	}

}