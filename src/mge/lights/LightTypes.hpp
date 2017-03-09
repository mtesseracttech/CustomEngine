#ifndef LIGHTTYPES_H
#define LIGHTTYPES_H


#include "Light.hpp"
#include "mge/core/World.hpp"
#include "GL/glew.h"
#include "mge/core/ShaderProgram.hpp"
//#include "mge/config.hpp"
#include "mge/core/Mesh.hpp"


class DirectionalLight : public Light {
public:
	DirectionalLight(std::string pName, glm::vec3 pPosition, glm::vec3 pColor, glm::vec3 pDirection);
	static ShaderProgram* _depthDirShader;
	GLuint depthDirMap;
	glm::mat4 lightSpaceMatrix;
	glm::vec3 position;

	void renderShadowMap();
private:
	World *_world;

	GLuint depthDirMapFBO;

	glm::vec3 target;
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	const GLuint SCREEN_WIDTH = 1920, SCREEN_HEIGHT = 1080;
	GLfloat near_plane = 1.0f, far_plane = 20.0f;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;
	static GLint _uLightSpaceMatrix;
	static GLint _uModel;

	void _renderDepthMap(GameObject* object);
};

class PointLight : public Light {
public:
	PointLight(std::string pName, glm::vec3 pPosition, glm::vec3 pColor);
	static ShaderProgram *_depthPointShader;
	//'Light': no appropriate default constructor available				error
	//virtual ~PointLight();
	void renderShadowMap();
	GLuint poiDepthCubeMap;

	glm::vec3 pointLightPosition;
	GLfloat farPlane = 20.0f;
private:
	GLuint depthCubeMapFBO;
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	const GLuint SCREEN_WIDTH = 1920, SCREEN_HEIGHT = 1080;
	GLfloat nearPlane = 1.0f;
	std::vector<glm::mat4> shadowTransforms;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;
	static GLint _uModel;
	static GLint _uLightPos;
	static GLint _uFarPlane;

	void _renderDepthMap(GameObject* object);
};

class SpotLight : public Light {
public:
	SpotLight(std::string pName, glm::vec3 pPosition, glm::vec3 pColor, glm::vec3 pDirection);
};

#endif // LIGHTTYPES_H