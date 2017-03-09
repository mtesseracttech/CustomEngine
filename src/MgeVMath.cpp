#include "MgeVMath.h"
#include <glm/detail/type_mat.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/orthonormalize.inl>
#include <cmath>


glm::vec3 MgeVMath::btToGlmVec(btVector3& vec)
{
	return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
}

btVector3 MgeVMath::glmToBtVec(glm::vec3& vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

glm::vec3 MgeVMath::glmVec3Right()
{
	return glm::vec3(1, 0, 0);
}

glm::vec3 MgeVMath::glmVec3Up()
{
	return glm::vec3(0, 1, 0);
}

glm::vec3 MgeVMath::glmVec3Forward()
{
	return glm::vec3(0, 0, -1);
}

glm::quat MgeVMath::QuatLookRotation(const glm::vec3 fwd, const glm::vec3 up)
{
	glm::vec3 z = glm::normalize(fwd);
	glm::vec3 x = glm::cross(glm::normalize(up), z);
	glm::vec3 y = glm::cross(z, x);
	return glm::quat_cast(glm::mat3(x, y, z));
}