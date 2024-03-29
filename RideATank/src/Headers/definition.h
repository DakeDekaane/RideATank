/*
 * definition.h
 *
 *  Created on: 06/09/2016
 *      Author: rey
 */

#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

typedef struct _VertexColor {

	_VertexColor() {
	}

	_VertexColor(glm::vec3 position, glm::vec3 color) {
		this->position = position;
		this->color = color;
	}

	glm::vec3 position;
	glm::vec3 color;
} VertexColor;

typedef struct _VertexLightColor {

	_VertexLightColor() {
	}

	_VertexLightColor(glm::vec3 position, glm::vec3 color, glm::vec3 normal) {
		this->position = position;
		this->color = color;
		this->normal = normal;
	}

	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
} VertexLightColor;

typedef struct _VertexLightTexture {

	_VertexLightTexture() {
	}

	_VertexLightTexture(glm::vec3 position, glm::vec2 texture,
			glm::vec3 normal) {
		this->position = position;
		this->texture = texture;
		this->normal = normal;
	}

	glm::vec3 position;
	glm::vec2 texture;
	glm::vec3 normal;
} VertexLightTexture;

enum MODEL_MODE {
	VERTEX,
	VERTEX_COLOR,
	VERTEX_TEXTURE,
	VERTEX_LIGHT_COLOR,
	VERTEX_LIGHT_TEXTURE
};

enum Direction {
	COLLISION_FRONT,
	COLLISION_BACK,
	COLLISION_RIGHT,
	COLLISION_LEFT
};

typedef struct _AABB {
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 getCenter() {
		GLfloat x = (max.x + min.x) / 2;
		GLfloat y = (max.y + min.y) / 2;
		GLfloat z = (max.z + min.z) / 2;
		return { x, y, z };
	}
} AABB;

typedef struct _SBB {
	glm::vec3 center;
	float ratio;
} SBB;

