#include "Light.hpp"

Light::Light(std::string pName, glm::vec3 pPosition, glm::vec3 pColor) : color(pColor), GameObject(pName, pPosition)
{
}

Light::~Light()
{
}