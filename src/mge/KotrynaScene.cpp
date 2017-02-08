#include <glm.hpp>
#include <iostream>
#include <string>
#include "behaviours/MovementBehavior.h"
#include "../../../../2.2/3d rendering/Engine/mge_v17_student_version/src/mge/behaviours/OrbitBehaviour.hpp"

using namespace std;

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"

#include "mge/core/Camera.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/KotrynaScene.h"


//construct the game class into _window, _renderer and hud (other parts are initialized by build)
KotrynaScene::KotrynaScene() :AbstractGame(), _hud(0)
{
}

void KotrynaScene::initialize() {
	//setup the core part
	AbstractGame::initialize();

	//setup the custom part
	cout << "Initializing HUD" << endl;
	_hud = new DebugHud(_window);
	cout << "HUD initialized." << endl << endl;
}

//build the game _world
void KotrynaScene::_initializeScene()
{
	_renderer->setClearColor(0, 0, 0);

	Camera* camera = new Camera("camera", glm::vec3(0, 0, 0));
	//camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);

	//MESHES
	//each mesh only has to be loaded once, but can be used multiple times:
	Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");//plane_8192
	Mesh* player = Mesh::load(config::MGE_MODEL_PATH + "bb81.obj");//cube_smooth

	//MATERIALS
	AbstractMaterial* colorMaterial = new ColorMaterial(glm::vec3(0.2f, 0.2f, 0.2f));
	AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "water.jpg"));
	AbstractMaterial* brickMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bb8.jpg"));

	//SCENE SETUP

	GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
	plane->scale(glm::vec3(100, 1, 100));
	//plane->scale(glm::vec3(0.5f, 0.5f, 0.5f));
	plane->setMesh(planeMeshDefault);
	plane->setMaterial(textureMaterial);
	_world->add(plane);

	GameObject* cube = new GameObject("player", glm::vec3(0, 1, 0));
	//cube->scale(glm::vec3(5, 5, 5));
	cube->scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cube->setMesh(player);
	cube->setMaterial(brickMaterial);
	_world->add(cube);
	cube->setBehaviour(new MovementBehavior(300,300));

	
	//camera->setBehaviour(new LookAt(plane,20));
	camera->setBehaviour(new OrbitBehaviour(150, cube));
	
}

void KotrynaScene::_render() {
	AbstractGame::_render();
	_updateHud();
}

void KotrynaScene::_updateHud() {
	string debugInfo = "";
	debugInfo += string("FPS:") + std::to_string((int)_fps) + "\n";

	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

KotrynaScene::~KotrynaScene()
{}
