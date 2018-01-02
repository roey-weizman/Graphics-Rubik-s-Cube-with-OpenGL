#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <Windows.h>
#include <list>
#include <iostream>
#include "display.h"
#include "mesh.h"
class lasTransformState
{
public:
	lasTransformState();
	~lasTransformState();
	void setTranslateMatrix(glm::mat4 tranMat);
	void setRotationMatrix(glm::mat4 rotateMat);
	glm::mat4 getRotationMat();
	glm::mat4 getTranslateMat();
private:
	glm::mat4 lastTranslation;
	glm::mat4 lastRotation;
};

