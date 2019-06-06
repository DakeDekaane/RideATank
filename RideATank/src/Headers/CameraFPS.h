/*
 * CameraFPS.h
 *
 *  Created on: 06/09/2016
 *      Author: rey
 */
#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	C_FORWARD, C_BACKWARD, C_LEFT, C_RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVTY = 0.25f;
const float ZOOM = 45.0f;

class CameraFPS {
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//Sky Camera
	glm::vec3 skyCameraPos;
	glm::vec3 skyCameraPosOffset = { 10.0f, 20.0f, 10.0f };
	glm::vec3 skyCameraFront;
	glm::vec3 skyCameraUp;

	// Eular Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	CameraFPS(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :
			Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(
					SENSITIVTY), Zoom(ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// Constructor with scalar values
	CameraFPS(float posX, float posY, float posZ, float upX, float upY,
			float upZ, float yaw, float pitch) :
			Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(
					SENSITIVTY), Zoom(ZOOM) {
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() {
		glm::vec3 cameraOffsetBase(5.0f, 2.0f, 1.0f);
		glm::vec3 cameraOffset;
		cameraOffset.x = - sin(glm::radians(Yaw))*cameraOffsetBase.z - cos(glm::radians(Yaw))*cameraOffsetBase.x;
		cameraOffset.y = cameraOffsetBase.y;
		cameraOffset.z = cos(glm::radians(Yaw))*cameraOffsetBase.z - sin(glm::radians(Yaw))*cameraOffsetBase.x;
		return glm::lookAt(Position + cameraOffset, Position + cameraOffset + Front, Up);
	}

	glm::mat4 GetViewMatrixSky() {
		Pitch = 0.0f;
		skyCameraPos = Position + skyCameraPosOffset;
		skyCameraFront = skyCameraPos - skyCameraPosOffset;
		skyCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		return glm::lookAt(skyCameraPos, skyCameraFront, skyCameraUp);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = MovementSpeed * deltaTime;
		float y0 = Position.y;
		if (direction == C_FORWARD)
			Position += Front * velocity;
		if (direction == C_BACKWARD)
			Position -= Front * velocity;
		if (direction == C_LEFT)
			Position -= Right * velocity;
		if (direction == C_RIGHT)
			Position += Right * velocity;
		Position.y = y0;
	}

	void ProcessMouseMovement(float xoffset, float yoffset,	GLboolean constrainPitch = true) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		Yaw += xoffset;
		Pitch += yoffset;
		if (constrainPitch) {
			if (Pitch > 14.0f)
				Pitch = 14.0f;
			if (Pitch < -3.0f)
				Pitch = -3.0f;
		}
		updateCameraVectors();
	}

private:
	void updateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
