/*
* collision.h
*
*  Created on: 13/10/2016
*      Author: rey
*/

#pragma once

#include <float.h>
#include "Mesh.h"
#include "definition.h"

AABB getAABB(std::vector<Mesh> meshes);

SBB getSBB(std::vector<Mesh> meshes);

bool raySphereIntersect(glm::vec3 orig, glm::vec3 dest, glm::vec3 dir, glm::vec3 vSphereCenter, float sphereRadius, float &t);

bool testSphereSphereIntersection(SBB sbb1, SBB sbb2);

bool testBoxBoxIntersection(AABB aabb1, AABB aabb2);

Direction getCollisionDirection2D(glm::vec3 model1, glm::vec3 model2, float yaw);
