#pragma once
#include "LinearMath/btVector3.h"
#include <glm/detail/type_vec3.hpp>
#include <glm/gtx/quaternion.hpp>

class MgeVMath
{
public:
	static glm::vec3 btToGlmVec(btVector3& vec);
	static btVector3 glmToBtVec(glm::vec3& vec);
	static glm::vec3 glmVec3Right();
	static glm::vec3 glmVec3Up();
	static glm::vec3 glmVec3Forward();
	static glm::quat QuatLookRotation(glm::vec3 fwd, glm::vec3 up);
private:
	MgeVMath() = delete;
	~MgeVMath() = delete;
};

