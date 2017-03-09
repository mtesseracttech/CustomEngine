#include "LightTypes.hpp"
#include <glm.hpp>
#include "mge/config.hpp"

ShaderProgram*	DirectionalLight::_depthDirShader = NULL;
GLint			DirectionalLight::_uLightSpaceMatrix = 0;
GLint			DirectionalLight::_uModel = 0;
GLint			DirectionalLight::_aVertex = 0;
GLint			DirectionalLight::_aNormal = 0;
GLint			DirectionalLight::_aUV = 0;

ShaderProgram* PointLight::_depthPointShader = NULL;
GLint			PointLight::_uModel;
GLint			PointLight::_uLightPos;
GLint			PointLight::_uFarPlane;
GLint			PointLight::_aVertex = 0;
GLint			PointLight::_aNormal = 0;
GLint			PointLight::_aUV = 0;



DirectionalLight::DirectionalLight(std::string pName, glm::vec3 pPosition, glm::vec3 pColor, glm::vec3 pDirection) : Light(pName, pPosition, pColor)
{
	_world = World::instance;
	World::instance->registerLight(this);
	setTransformForward(pDirection);
	position = pPosition;
	target = pDirection;

	//2D texture
	glm::mat4 lightProjection, lightView;

	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	//lightView = glm::lookAt(pPosition, pPosition + glm::normalize(pDirection), glm::vec3(0.0, 1.0, 0.0));
	lightView = glm::lookAt(position, target, glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	//if (!_depthDirShader) {
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };

	glGenFramebuffers(1, &depthDirMapFBO);

	_depthDirShader = new ShaderProgram();
	_depthDirShader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "Shadows.vs");
	_depthDirShader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "Shadows.fs");
	_depthDirShader->finalize();

	_aVertex = _depthDirShader->getAttribLocation("vertex");
	_aNormal = _depthDirShader->getAttribLocation("normal");
	_aUV = _depthDirShader->getAttribLocation("uv");

	_uLightSpaceMatrix = _depthDirShader->getUniformLocation("lightSpaceMatrix");
	_uModel = _depthDirShader->getUniformLocation("model");

	//2D depth map Texture
	glGenTextures(1, &depthDirMap);
	glBindTexture(GL_TEXTURE_2D, depthDirMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthDirMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthDirMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//}

	renderShadowMap();
}

void DirectionalLight::renderShadowMap()
{
	_depthDirShader->use();

	glUniformMatrix4fv(_uLightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthDirMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	//Render here
	_renderDepthMap(_world);
	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

PointLight::PointLight(std::string pName, glm::vec3 pPosition, glm::vec3 pColor) : Light(pName, pPosition, pColor)
{

	World::instance->registerLight(this);

	//Configure shadows
	pointLightPosition = pPosition;

	//CubeMap
	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);

	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));


	//Set up shader
	_depthPointShader = new ShaderProgram();
	_depthPointShader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "ShadowsCube.vs");
	_depthPointShader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "ShadowsCube.fs");
	_depthPointShader->addShader(GL_GEOMETRY_SHADER, config::MGE_SHADER_PATH + "ShadowsCube.gs");
	_depthPointShader->finalize();

	_aVertex = _depthPointShader->getAttribLocation("vertex");
	_aNormal = _depthPointShader->getAttribLocation("normal");
	_aUV = _depthPointShader->getAttribLocation("uv");

	glGenFramebuffers(1, &depthCubeMapFBO);

	//Cube depth map
	glGenTextures(1, &poiDepthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, poiDepthCubeMap);

	_uModel = _depthPointShader->getUniformLocation("model");
	_uLightPos = _depthPointShader->getUniformLocation("lightPos");
	_uFarPlane = _depthPointShader->getUniformLocation("far_plane");

	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, poiDepthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		std::cout << "Framebuffer not complete!   " << Status << "   " << SHADOW_HEIGHT << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Not ok" << std::endl;
	}
	else {
		std::cout << "All OK" << std::endl;
	}


	renderShadowMap();
}

void PointLight::renderShadowMap()
{
	//RENDER
	_depthPointShader->use();
	glUniform3fv(_uLightPos, 1, glm::value_ptr(pointLightPosition));
	glUniform1f(_uFarPlane, farPlane);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (GLuint i = 0; i < 6; ++i) {
		glUniformMatrix4fv(_depthPointShader->getUniformLocation("shadowTransforms[" + std::to_string(i) + "]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
	}

	glCullFace(GL_FRONT);
	//Render here
	_renderDepthMap(World::instance);
	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

SpotLight::SpotLight(std::string pName, glm::vec3 pPosition, glm::vec3 pColor, glm::vec3 pDirection) : Light(pName, pPosition, pColor)
{
	World::instance->registerLight(this);
	setTransformForward(pDirection);
}


void DirectionalLight::_renderDepthMap(GameObject* object) {

	int childCount = object->getChildCount();
	if (childCount < 1) return;


	GameObject* child = 0;
	for (int i = 0; i < childCount; i++) {
		child = object->getChildAt(i);

		if (child->getMesh() != NULL && child->getMaterial() != NULL) {
			glUniformMatrix4fv(_uModel, 1, GL_FALSE, glm::value_ptr(child->getWorldTransform()));
			child->getMesh()->streamToOpenGL(_aVertex, _aNormal, _aUV);

		}
		_renderDepthMap(child);
	}
}

void PointLight::_renderDepthMap(GameObject* object) {

	int childCount = object->getChildCount();
	if (childCount < 1) return;


	GameObject* child = 0;
	for (int i = 0; i < childCount; i++) {
		child = object->getChildAt(i);

		if (child->getMesh() != NULL && child->getMaterial() != NULL) {
			glUniformMatrix4fv(_uModel, 1, GL_FALSE, glm::value_ptr(child->getWorldTransform()));
			child->getMesh()->streamToOpenGL(_aVertex, _aNormal, _aUV);

		}
		_renderDepthMap(child);
	}
}