#include <iostream>

#include "Headers/InputManager.h"
#include "Headers/TimeManager.h"

InputCodes InputManager::toApplicationKey(int key) {
	switch (key) {
	case 256:
		return InputCodes::kEscape;
	case 32:
		return InputCodes::Space;
	case 87:
		return InputCodes::W;
	case 83:
		return InputCodes::S;
	case 65:
		return InputCodes::A;
	case 68:
		return InputCodes::D;
	case 265:
		return InputCodes::Up;
	case 264:
		return InputCodes::Down;
	case 263:
		return InputCodes::Left;
	case 262:
		return InputCodes::Right;
	case 67:
		return InputCodes::C;
	case 78:
		return InputCodes::N;
	case 77:
		return InputCodes::M;
	case 49:
		return InputCodes::ONE;
	case 50:
		return InputCodes::TWO;
	case 69:
		return InputCodes::E;
	case 81:
		return InputCodes::Q;
	}
}

State InputManager::toApplicationState(int state) {
	switch (state) {
	case 0:
		return State::RELESED;
	case 1:
		return State::PRESSED;
	case 2:
		return State::REPEATED;
	}
}

MouseButtonIndex InputManager::toMouseButtonIndex(int button) {
	switch (button) {
	case 0:
		return MouseButtonIndex::LEFT;
	case 1:
		return MouseButtonIndex::RIGHT;
	case 2:
		return MouseButtonIndex::MIDDLE;
	}
}

void InputManager::keyPressed(InputCodes code, float deltaTime, State state) {
	keyState[code] = state;
}

void InputManager::mouseMoved(float mouseX, float mouseY) {
	float xoffset = mouseX - lastMousePos.x;
	float yoffset = lastMousePos.y - mouseY;
	cameraFPS->ProcessMouseMovement(xoffset, yoffset, true);
	lastMousePos = glm::vec2(mouseX, mouseY);
}

void InputManager::mouseClicked(MouseButtonIndex code, float mouseX,
	float mouseY, State state) {
	mouseButtomState[code] = state;
	switch (code) {
	case RIGHT:
		std::cout << "lastMousePos.x:" << lastMousePos.x << std::endl;
		break;
	case LEFT:
		std::cout << "lastMousePos.y:" << lastMousePos.y << std::endl;
		lastMousePos.x = mouseX;
		lastMousePos.y = mouseY;
		break;
	case MIDDLE:
		std::cout << "lastMousePos.x:" << lastMousePos.x << std::endl;
		std::cout << "lastMousePos.y:" << lastMousePos.y << std::endl;
		break;
	}
}

void InputManager::mouseScroll(float yoffset) {
}

void InputManager::do_movement(float deltaTime) {
	if (keyState[InputCodes::W] || (collision && direction != COLLISION_FRONT))
		cameraFPS->ProcessKeyboard(C_FORWARD, deltaTime);
	if (keyState[InputCodes::S] || (collision && direction != COLLISION_BACK))
		cameraFPS->ProcessKeyboard(C_BACKWARD, deltaTime);
}

void InputManager::swapCamera() {
	if (keyState[InputCodes::ONE]) {
		activeCamera = PLAYER;
		std::cout << "Camara FPS selecconada" << std::endl;
	}
	else if (keyState[InputCodes::TWO] ) {
		activeCamera = SKY;
		std::cout << "Camara area seleccionada" << std::endl;
	}
}

void InputManager::changeDay() {
	if (keyState[InputCodes::N] ) {
		_isDay = false;
	}
	if (keyState[InputCodes::M]) {
		_isDay = true;
	}
}

void InputManager::changeTankLights() {
	if (keyState[InputCodes::Q]) {
		tank_lights = true;
	}
	if (keyState[InputCodes::E]) {
		tank_lights = false;
	}
}



