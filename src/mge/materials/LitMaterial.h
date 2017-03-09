#ifndef LITMATERIAL_H
#define LITMATERIAL_H

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"


class LitMaterial : public AbstractMaterial
{
public:
	/**
	 * \brief 
	 * \param DiffuseTexture
	 * \param SpeculatMap 
	 * \param useSpecMap 
	 * \param EmissionMap 
	 * \param UseEmission 
	 * \param Color 
	 */
	LitMaterial(Texture* pDiffuseTexture, Texture* pSpeculatMap,bool useSpecMap = false, Texture* pEmissionMap = nullptr, bool pUseEmission = false,glm::vec3 pColor = glm::vec3(1, 0, 0));
	virtual ~LitMaterial();

	virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	//in rgb values
	void setDiffuseColor(glm::vec3 pDiffuseColor);
	void setDiffuseTexture(Texture* pDiffuseTexture);
	void useTextures(bool pInUse);

private:

	void _initializeLights();

	//all the static properties are shared between instances of ColorMaterial
	//note that they are all PRIVATE, we do not expose this static info to the outside world
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uMVPMatrix;

	static GLint _uLightSpaceMatrix2;
	static GLint _uDiffuseColor;
	static GLint _uModelMatrix;
	static GLint _uModel;

	static GLint _uCameraPosition;
	static GLint _uMaterialShininess;
	static GLint _uDirectionalLightCount;
	static GLint _uPointLightCount;
	static GLint _uSpotLightCount;
	//Material
	static GLint _uMaterialSpecular;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	static GLint _lightPos;
	static GLint _farPlane;
	static GLint _farPlane2;

	Texture* _diffuseTexture;
	Texture* _specularTexture;
	Texture* _emissionTexture;
	bool _useSpecular;
	bool _useEmission;
	//this one is unique per instance of color material
	glm::vec3 _diffuseColor;

	World *_world;

	std::vector<DirectionalLight*> *_directionalLights;
	std::vector<PointLight*> *_pointLights;
	std::vector<SpotLight*> *_spotLights;

	//2D
	glm::mat4 lightSpaceMatrix;
	//Cube
	GLfloat farPlaneCube;
	std::vector<glm::mat4> shadowTransforms;


	int iteraror = 0;
};

class DirectionalLightInfo {

public:
	DirectionalLightInfo(int index, ShaderProgram *_shader);

	GLint direction;
	GLint color;
	GLint dirDepthMap;
	GLint lightSpaceMatrix;
	GLint position;
};

class PointLightInfo {

public:
	PointLightInfo(int index, ShaderProgram *_shader);

	GLint position;
	GLint color;

	GLint constant;
	GLint linear;
	GLint quadratic;

	GLint farPlane;
	GLint pointCubeDepthMap;
};

class SpotLightInfo {

public:
	SpotLightInfo(int index, ShaderProgram *_shader);

	GLint position;
	GLint direction;
	GLint color;

	GLint constant;
	GLint linear;
	GLint quadratic;

	GLint cutOff;
	GLint outerCutOff;
};

class Cache {

public:

	static Cache *cache;
	static void Build(ShaderProgram *_shader);

	vector<DirectionalLightInfo> *directionalLightInfo;
	vector<PointLightInfo> *pointLightInfo;
	vector<SpotLightInfo> *spotLightInfo;

private:
	Cache(ShaderProgram* _shader);
	~Cache();
};

#endif // LITMATERIAL_H