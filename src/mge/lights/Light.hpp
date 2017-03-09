#ifndef LIGHT_H
#define LIGHT_H

#include <mge/core/GameObject.hpp>

class Light : public GameObject
{
public:
	Light(std::string pName, glm::vec3 pPosition, glm::vec3 pColor);
	~Light();

	glm::vec3 color;
private:


};

#endif // LIGHT_H