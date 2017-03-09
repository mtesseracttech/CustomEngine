#include "mge/core/World.hpp"

using namespace std;

World *World::instance;

World::World() :GameObject("root"), _mainCamera(0)
{
	//ctor
	World::instance = this;
	_staticDirectionalLights = new vector<DirectionalLight*>();
	_staticPointLights = new vector<PointLight*>();
	_staticSpotLights = new vector<SpotLight*>();
}

void World::setMainCamera(Camera* pCamera) {
	if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera() {
	return _mainCamera;
}


void World::registerLight(DirectionalLight * pLight)
{
	_staticDirectionalLights->push_back(pLight);
}

void World::registerLight(PointLight * pLight)
{
	_staticPointLights->push_back(pLight);
}

void World::registerLight(SpotLight * pLight)
{
	_staticSpotLights->push_back(pLight);
}


std::vector<DirectionalLight*>* World::getStaticDirectionalLights()
{
	return _staticDirectionalLights;
}

std::vector<PointLight*>* World::getStaticPointLights()
{
	return _staticPointLights;
}

std::vector<SpotLight*>* World::getStaticSpotLights()
{
	return _staticSpotLights;
}

void World::update(float pStep)
{
	GameObject::update(pStep);
	iterator++;
}

int World::getIterator()
{
	return iterator;
}

