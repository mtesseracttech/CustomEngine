#include <glm.hpp>
#include "mge/materials/LitMaterial.h"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"

Cache *Cache::cache;

ShaderProgram* LitMaterial::_shader = NULL;

//Uniform
GLint LitMaterial::_uMVPMatrix = 0;

GLint LitMaterial::_uLightSpaceMatrix2 = 0;
GLint LitMaterial::_uDiffuseColor = 0;
GLint LitMaterial::_uModelMatrix = 0;
GLint LitMaterial::_uCameraPosition = 0;
GLint LitMaterial::_uDirectionalLightCount = 0;
GLint LitMaterial::_uPointLightCount = 0;
GLint LitMaterial::_uSpotLightCount = 0;
GLint LitMaterial::_uModel = 0;
//Materials
GLint LitMaterial::_uMaterialShininess = 0;
GLint LitMaterial::_uMaterialSpecular = 0;

//Attribute
GLint LitMaterial::_aVertex = 0;
GLint LitMaterial::_aNormal = 0;
GLint LitMaterial::_aUV = 0;

GLint LitMaterial::_lightPos = 0;
GLint LitMaterial::_farPlane = 0;
GLint LitMaterial::_farPlane2 = 0;



GLuint depthCubeMapFBO;


const GLfloat constant = 0.5f;
const GLfloat linear = 0.5f;
const GLfloat quadratic = 0.0f;
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const GLuint SCREEN_WIDTH = 1920, SCREEN_HEIGHT = 1080;
GLuint depthMap;
GLuint depthCubeMap;

GLuint shadowMap;


float timer = 0;

LitMaterial::LitMaterial(Texture* pDiffuse, Texture* pSpecularMap,bool pUseSpecMap, Texture* pEmissionMap, bool pUseEmissionMap,glm::vec3 pDiffuseColor) :
																	_diffuseTexture(pDiffuse),_specularTexture(pSpecularMap),_useSpecular(pUseSpecMap),_emissionTexture(pEmissionMap),_useEmission(pUseEmissionMap)
																	,_diffuseColor(pDiffuseColor)
{
	_world = World::instance;
	//every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
	_lazyInitializeShader();
	_initializeLights();

}

void LitMaterial::_lazyInitializeShader() {
	//this shader contains everything the material can do (it can render something in 3d using a single color)
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "lit.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "lit.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uDiffuseColor = _shader->getUniformLocation("diffuseColor");

		_uModelMatrix = _shader->getUniformLocation("modelMatrix");
		_uCameraPosition = _shader->getUniformLocation("cameraPosition");
		_uDirectionalLightCount = _shader->getUniformLocation("directionalLightCount");
		_uPointLightCount = _shader->getUniformLocation("pointLightCount");
		_uSpotLightCount = _shader->getUniformLocation("spotLightCount");
		//Material
		_uMaterialShininess = _shader->getUniformLocation("material.shininess");
		_uMaterialSpecular = _shader->getUniformLocation("material.specular");
		_uLightSpaceMatrix2 = _shader->getUniformLocation("lightSpaceMatrix");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");

		_farPlane2 = _shader->getUniformLocation("far_plane");
	}
}

void LitMaterial::_initializeLights()
{
	_directionalLights = _world->getStaticDirectionalLights();
	_pointLights = _world->getStaticPointLights();
	_spotLights = _world->getStaticSpotLights();
}

LitMaterial::~LitMaterial()
{
	//dtor
}

void LitMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) {
	_diffuseColor = pDiffuseColor;
}

void LitMaterial::setDiffuseTexture(Texture * pDiffuseTexture)
{
	_diffuseTexture = pDiffuseTexture;
}

void LitMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	Cache::Build(_shader);
	timer++;
	/**/
	if (timer > 60) {
		for (int i = 0; i < _directionalLights->size(); i++)
		{
			_directionalLights->at(i)->renderShadowMap();
		}

		for (int i = 0; i < _pointLights->size(); i++)
		{
			//_pointLights->at(i)->renderShadowMap();
		}
		timer = 0;
	}

	/**/
	_shader->use();

	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	glUniform1i(_shader->getUniformLocation("textureDiffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _specularTexture->getId());
	glUniform1i(_shader->getUniformLocation("specularMap"), 1);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _emissionTexture->getId());
	glUniform1i(_shader->getUniformLocation("emissionMap"), 2);

	glUniform1i(_shader->getUniformLocation("useSpecular"), _useSpecular);
	glUniform1i(_shader->getUniformLocation("useEmission"), _useEmission);

	for (int i = 0; i < _pointLights->size() && i < 4; i++) {
		glActiveTexture(GL_TEXTURE12 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _pointLights->at(i)->poiDepthCubeMap);
		glUniform1i(_shader->getUniformLocation("pointCubeDepthMap[" + to_string(i) + "]"), 12 + i);
	}


	glUniform1f(_farPlane2, farPlaneCube);

	//pass in a precalculate mvp matrix (see texture material for the opposite)	
	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniformMatrix4fv(_uModelMatrix, 1, GL_FALSE, glm::value_ptr(pModelMatrix));

	//set the material color
	glUniform3fv(_uDiffuseColor, 1, glm::value_ptr(_diffuseColor));
	//Camera Position
	glm::vec3 cameraPosition = _world->getMainCamera()->getWorldPosition();
	glUniform3fv(_uCameraPosition, 1, glm::value_ptr(cameraPosition));
	//Material
	glUniform1f(_uMaterialShininess, 256);
	glUniform3f(_uMaterialSpecular, 1.0f, 1.0f, 1.0f);

	//Number of lights
	glUniform1i(_uDirectionalLightCount, _directionalLights->size());
	glUniform1i(_uPointLightCount, _pointLights->size());
	glUniform1i(_uSpotLightCount, _spotLights->size());

	//Directional Light
	for (int i = 0; i < _directionalLights->size() && i < 4; i++) {
		glUniform3fv(Cache::cache->directionalLightInfo->at(i).direction, 1, glm::value_ptr(_directionalLights->at(i)->getWorldTransformForward()));
		glUniform3fv(Cache::cache->directionalLightInfo->at(i).color, 1, glm::value_ptr(_directionalLights->at(i)->color));

		glActiveTexture(GL_TEXTURE9 + i);
		glBindTexture(GL_TEXTURE_2D, _directionalLights->at(i)->depthDirMap);
		glUniform1i(Cache::cache->directionalLightInfo->at(i).dirDepthMap, 9 + i);

		glUniformMatrix4fv(Cache::cache->directionalLightInfo->at(i).lightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(_directionalLights->at(i)->lightSpaceMatrix));
		glUniform3fv(Cache::cache->directionalLightInfo->at(i).position, 1, glm::value_ptr(_directionalLights->at(i)->position));
	}

	//Point Light
	for (int i = 0; i < _pointLights->size() && i < 4; i++) {
		glUniform3fv(Cache::cache->pointLightInfo->at(i).position, 1, glm::value_ptr(_pointLights->at(i)->getWorldPosition()));
		glUniform3fv(Cache::cache->pointLightInfo->at(i).color, 1, glm::value_ptr(_pointLights->at(i)->color));
		glUniform1f(Cache::cache->pointLightInfo->at(i).constant, constant);
		glUniform1f(Cache::cache->pointLightInfo->at(i).linear, linear);
		glUniform1f(Cache::cache->pointLightInfo->at(i).quadratic, quadratic);
		glUniform1f(Cache::cache->pointLightInfo->at(i).farPlane, _pointLights->at(i)->farPlane);
	}

	//Spot Light
	for (int i = 0; i < _spotLights->size() && i < 4; i++) {
		glUniform3fv(Cache::cache->spotLightInfo->at(i).position, 1, glm::value_ptr(_spotLights->at(i)->getWorldPosition()));
		glUniform3fv(Cache::cache->spotLightInfo->at(i).direction, 1, glm::value_ptr(_spotLights->at(i)->getWorldTransformForward()));
		glUniform3fv(Cache::cache->spotLightInfo->at(i).color, 1, glm::value_ptr(_spotLights->at(i)->color));
		glUniform1f(Cache::cache->spotLightInfo->at(i).constant, constant);
		glUniform1f(Cache::cache->spotLightInfo->at(i).linear, linear);
		glUniform1f(Cache::cache->spotLightInfo->at(i).quadratic, quadratic);
		glUniform1f(Cache::cache->spotLightInfo->at(i).cutOff, glm::cos(glm::radians(15.0f)));
		glUniform1f(Cache::cache->spotLightInfo->at(i).outerCutOff, glm::cos(glm::radians(20.0f)));
	}

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);

	glGetError();
}

DirectionalLightInfo::DirectionalLightInfo(int index, ShaderProgram * _shader)
{
	direction = _shader->getUniformLocation("directionalLight[" + to_string(index) + "].direction");
	color = _shader->getUniformLocation("directionalLight[" + to_string(index) + "].color");
	dirDepthMap = _shader->getUniformLocation("directionalLight[" + to_string(index) + "].dirDepthMap");
	lightSpaceMatrix = _shader->getUniformLocation("directionalLight[" + to_string(index) + "].LSM");
	position = _shader->getUniformLocation("directionalLight[" + to_string(index) + "].position");
}

PointLightInfo::PointLightInfo(int index, ShaderProgram * _shader)
{
	position = _shader->getUniformLocation("pointLight[" + to_string(index) + "].position");
	color = _shader->getUniformLocation("pointLight[" + to_string(index) + "].color");
	constant = _shader->getUniformLocation("pointLight[" + to_string(index) + "].constant");
	linear = _shader->getUniformLocation("pointLight[" + to_string(index) + "].linear");
	quadratic = _shader->getUniformLocation("pointLight[" + to_string(index) + "].quadratics");
	farPlane = _shader->getUniformLocation("pointLight[" + to_string(index) + "].farPlane");
}

SpotLightInfo::SpotLightInfo(int index, ShaderProgram * _shader)
{
	position = _shader->getUniformLocation("spotLight[" + to_string(index) + "].position");
	direction = _shader->getUniformLocation("spotLight[" + to_string(index) + "].direction");
	color = _shader->getUniformLocation("spotLight[" + to_string(index) + "].color");
	constant = _shader->getUniformLocation("spotLight[" + to_string(index) + "].constant");
	linear = _shader->getUniformLocation("spotLight[" + to_string(index) + "].linear");
	quadratic = _shader->getUniformLocation("spotLight[" + to_string(index) + "].quadratics");
	cutOff = _shader->getUniformLocation("spotLight[" + to_string(index) + "].cutOff");
	outerCutOff = _shader->getUniformLocation("spotLight[" + to_string(index) + "].outerCutOff");
}

void Cache::Build(ShaderProgram * _shader)
{
	if (!cache) {
		cache = new Cache(_shader);
	}
}

Cache::Cache(ShaderProgram * _shader)
{
	directionalLightInfo = new vector<DirectionalLightInfo>();
	pointLightInfo = new vector<PointLightInfo>();
	spotLightInfo = new vector<SpotLightInfo>();

	for (int i = 0; i <= World::instance->getStaticDirectionalLights()->size() && i < 4; i++) {
		directionalLightInfo->push_back(DirectionalLightInfo(i, _shader));
	}
	for (int i = 0; i <= World::instance->getStaticPointLights()->size() && i < 4; i++) {
		pointLightInfo->push_back(PointLightInfo(i, _shader));
	}
	for (int i = 0; i < -World::instance->getStaticSpotLights()->size() && i < 4; i++) {
		spotLightInfo->push_back(SpotLightInfo(i, _shader));
	}
}

Cache::~Cache()
{
}


