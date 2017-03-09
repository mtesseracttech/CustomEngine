#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include "mge/lights/LightTypes.hpp"

class Camera;
class DirectionalLight;
class PointLight;
class SpotLight;

class World : public GameObject
{
public:
	World();

	static World *instance;

	void setMainCamera(Camera* pCamera);
	Camera* getMainCamera();

	void registerLight(DirectionalLight *pLight);
	void registerLight(PointLight *pLight);
	void registerLight(SpotLight *pLight);

	std::vector<DirectionalLight*> *getStaticDirectionalLights();
	std::vector<PointLight*> *getStaticPointLights();
	std::vector<SpotLight*> *getStaticSpotLights();

	void update(float pStep);

	int getIterator();

private:
	Camera* _mainCamera;

	World(const World&);
	World& operator=(const World&);

	std::vector<DirectionalLight*> *_staticDirectionalLights;
	std::vector<PointLight*> *_staticPointLights;
	std::vector<SpotLight*> *_staticSpotLights;

	int iterator = 0;
};


#endif // WORLD_H
