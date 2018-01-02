#include "lasTransformState.h"



lasTransformState::lasTransformState():lastRotation(glm::mat4(1)), lastTranslation(glm::mat4(1))
{
	
}

lasTransformState::~lasTransformState()
{
}

void lasTransformState::setTranslateMatrix(glm::mat4 tranMat)
{
	lastTranslation = glm::mat4(tranMat);
}

void lasTransformState::setRotationMatrix(glm::mat4 rotateMat)
{
	lastRotation = glm::mat4(rotateMat);
}

glm::mat4 lasTransformState::getRotationMat()
{
	return lastRotation;
}

glm::mat4 lasTransformState::getTranslateMat()
{
	return lastTranslation;
}
