#include "KotrynaScene.h"
#include "Physics/RigidBody.h"
#include "mge/core/Renderer.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"

#include "mge/core/Camera.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include "mge/materials/TextureMaterial.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"

#include "btBulletDynamicsCommon.h"
#include "mge/behaviours/OrbCamera.h"
#include "mge/behaviours/MovementBehaviour.h"
#include "mge/behaviours/OrbitBehavior.h"
#include "mge/behaviours/ShootingBehaviourPlayer.h"
#include "Physics/MgeRigidbody.h"
#include "materials/LitMaterial.h"
#include "behaviours/DoorBehavior.h"
#include "objects/Trigger.h"
#include "objects/Interactable.hpp"

using namespace std;

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
KotrynaScene::KotrynaScene() : AbstractGame(),_physicsWorld(nullptr),_musicPlayer()
{
	
}


void KotrynaScene::initialize()
{
	//Setting up the core part
	AbstractGame::initialize();

	//Setting up HUD
	cout << "Initializing HUD" << endl;
	DebugHud::SetupInstance(_window);
	_hud = DebugHud::GetInstance();

	//_hud = new DebugHud(_window);
//	_hud = DebugHud::getInstanceHUD();
	//_hud->setWindow(_window);
	cout << "HUD initialized." << endl << endl;
	_musicPlayer.Play(Music::Level1,true);
}

void KotrynaScene::_initializeWorld()
{
	cout << "Initializing Physics World!" << endl;
	_world = new PhysicsWorld();
	_physicsWorld = static_cast<PhysicsWorld*>(_world);
	cout << "Done Initializing Physics World!" << endl;
}
//build the game _world
void KotrynaScene::_initializeScene()
{
	_renderer->setClearColor(0, 0, 0); // color of background

    //CAMERA
	Camera* camera = new Camera("camera", glm::vec3(0, 6, 7));
	camera->rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0));
	_physicsWorld->add(camera);
	_physicsWorld->setMainCamera(camera);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////									MESH SETUP																								/////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh* level2Mesh = Mesh::load(config::MGE_MODEL_PATH    + "/Environment/Tutorial_Level_Floor.obj");
	Mesh* groundMesh = Mesh::load(config::MGE_MODEL_PATH    + "/Environment/Tutorial_Level_Floor_Transparent.obj");
	Mesh* floorDetail = Mesh::load(config::MGE_MODEL_PATH   + "/Environment/Tutorial_Level_Floor_Detail.obj");
	Mesh* airCondi = Mesh::load(config::MGE_MODEL_PATH		+ "/Environment/Air_Conditions.obj");
	Mesh* airCondiFan = Mesh::load(config::MGE_MODEL_PATH	+ "/Environment/Air_Conditions_Fan.obj");
	Mesh* airCondiGrid = Mesh::load(config::MGE_MODEL_PATH  + "/Environment/Air_Conditions_Grid.obj");
	Mesh* pipes1Mesh = Mesh::load(config::MGE_MODEL_PATH	+ "/Environment/Big_Pipes.obj");
	Mesh* pipes2Mesh = Mesh::load(config::MGE_MODEL_PATH	+ "/Environment/Small_Pipes_Combo_1.obj");
	Mesh* pipes3Mesh = Mesh::load(config::MGE_MODEL_PATH	+ "/Environment/Small_Pipes_Combo_2.obj");
	Mesh* pipes4Mesh = Mesh::load(config::MGE_MODEL_PATH	+ "/Environment/Small_Pipes_Combo_3.obj");
	Mesh* wallsLevel = Mesh::load(config::MGE_MODEL_PATH	+ "/Environment/Tutorial_Level_Walls.obj");
	Mesh* playerMesh = Mesh::load(config::MGE_MODEL_PATH	+ "/Player/Player.obj");
	Mesh* bulletMesh = Mesh::load(config::MGE_MODEL_PATH	+ "Bullet.obj");
	Mesh* enemiesMesh = Mesh::load(config::MGE_MODEL_PATH	+ "/Player/Enemy_1.obj");
	Mesh* enemiesMesh2 = Mesh::load(config::MGE_MODEL_PATH	+ "/Player/Enemy_2.obj");
	Mesh* doorMesh = Mesh::load(config::MGE_MODEL_PATH	    + "/Environment/Door.obj");
	Mesh* barrelMesh1 = Mesh::load(config::MGE_MODEL_PATH	+ "cube.obj");
	Mesh* ovenMesh = Mesh::load(config::MGE_MODEL_PATH		+ "/Environment/Oven.obj");
	Mesh* interactableMesh = Mesh::load(config::MGE_MODEL_PATH + "/Environment/Crate_Palette_1.obj");
	Mesh* triggerMesh = Mesh::load(config::MGE_MODEL_PATH + "/Environment/Crate_Palette_2.obj");
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////									MATERIALS																								/////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**/
	AbstractMaterial* woodMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH			+ "wood.png"));
	/**/
	

	//LIGHT MATERIAL
	AbstractMaterial* litMaterial = new LitMaterial(Texture::load(config::MGE_TEXTURE_PATH + "brickwall.jpg"), 
													Texture::load(config::MGE_TEXTURE_PATH + "/Player/lambert2SG_Specular.png"),false,
													Texture::load(config::MGE_TEXTURE_PATH + "/Player/lambert2SG_Specular.png"), false, 
													glm::vec3(0, 0, 0));

	AbstractMaterial* litMaterial1 = new LitMaterial(Texture::load(config::MGE_TEXTURE_PATH + "brickwall.jpg"), 
													 Texture::load(config::MGE_TEXTURE_PATH + "/Player/lambert2SG_Specular.png"), false,
													 Texture::load(config::MGE_TEXTURE_PATH + "/Player/lambert2SG_Specular.png"), false,
													 glm::vec3(0, 0, 0));

	AbstractMaterial* enemy1Material = new LitMaterial(Texture::load(config::MGE_TEXTURE_PATH + "/enemy1/lambert3SG_Diffuse.png"), 
													 Texture::load(config::MGE_TEXTURE_PATH + "/enemy1/lambert3SG_Specular.png"), true,
													 Texture::load(config::MGE_TEXTURE_PATH + "/enemy1/lambert3SG_Emissive.png"), true,
													 glm::vec3(0, 0, 0));

	AbstractMaterial* enemy2Material = new LitMaterial(Texture::load(config::MGE_TEXTURE_PATH + "/enemy2/lambert4SG_Diffuse.png"),
													   Texture::load(config::MGE_TEXTURE_PATH + "/enemy2/lambert4SG_Specular.png"), true,
													   Texture::load(config::MGE_TEXTURE_PATH + "/enemy2/lambert4SG_Emissive.png"), true,
												   	   glm::vec3(0, 0, 0));

	AbstractMaterial* litMaterial3 = new LitMaterial(Texture::load(config::MGE_TEXTURE_PATH + "wood.png"), 
													 Texture::load(config::MGE_TEXTURE_PATH + "/Player/lambert2SG_Specular.png"), false,
													 Texture::load(config::MGE_TEXTURE_PATH + "/Player/lambert2SG_Specular.png"), false,
													 glm::vec3(0, 0, 0));

	AbstractMaterial* playerMaterial = new LitMaterial(Texture::load(config::MGE_TEXTURE_PATH + "/Player/lambert2SG_Diffuse.png"),
													 Texture::load(config::MGE_TEXTURE_PATH + "/Player/lambert2SG_Specular.png"), true,
													 Texture::load(config::MGE_TEXTURE_PATH + "/Player/lambert2SG_Emissive.png"), true,
													 glm::vec3(0, 0, 0));

	AbstractMaterial* ovenMaterial = new LitMaterial(Texture::load(config::MGE_TEXTURE_PATH + "/Oven/lambert6SG_Diffuse.png"),
													 Texture::load(config::MGE_TEXTURE_PATH + "/Oven/lambert6SG_Specular.png"),true,
													 Texture::load(config::MGE_TEXTURE_PATH + "/Oven/lambert6SG_Emissive.png"), true,
													 glm::vec3(0, 0, 0));


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////									SCENE SETUP																								/////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**/
	//SOME PLANE AGAIN
	GameObject* plane2 = new GameObject("plane2", glm::vec3(0, 0, 0));
	plane2->setMesh(level2Mesh);
	plane2->setMaterial(litMaterial1);
	plane2->setMeshRigidBody(_physicsWorld->GetDynamicsWorld()); //ONLY USE THE MESHRIGIDBODY FOR STATIC OBJECTS!
	_physicsWorld->add(plane2);
	/**/
	GameObject* detailsOnGround = new GameObject("plane2", glm::vec3(0, 0, 0));
	detailsOnGround->setMesh(floorDetail);
	detailsOnGround->setMaterial(litMaterial1);
	detailsOnGround->setMeshRigidBody(_physicsWorld->GetDynamicsWorld()); //ONLY USE THE MESHRIGIDBODY FOR STATIC OBJECTS!
	_physicsWorld->add(detailsOnGround);
	/**/

	/**/
	//WALLS
	GameObject* walls = new GameObject("walls", glm::vec3(0, 0, 0));
	walls->setMesh(wallsLevel);
	walls->setMaterial(litMaterial1);
	walls->setMeshRigidBody(_physicsWorld->GetDynamicsWorld()); //ONLY USE THE MESHRIGIDBODY FOR STATIC OBJECTS!
	_physicsWorld->add(walls);
	/**/
	/**/
	//PIPES SOMEKINDOF
	GameObject* pipes = new GameObject("pipes1", glm::vec3(0, 0, 0));
	pipes->setMesh(pipes1Mesh);
	pipes->setMaterial(litMaterial);
	pipes->setMeshRigidBody(_physicsWorld->GetDynamicsWorld()); //ONLY USE THE MESHRIGIDBODY FOR STATIC OBJECTS!
	_physicsWorld->add(pipes);

	GameObject* pipes2 = new GameObject("pipes1", glm::vec3(0, 0, 0));
	pipes2->setMesh(pipes2Mesh);
	pipes2->setMaterial(litMaterial);
	pipes2->setMeshRigidBody(_physicsWorld->GetDynamicsWorld()); //ONLY USE THE MESHRIGIDBODY FOR STATIC OBJECTS!
	_physicsWorld->add(pipes2);

	GameObject* pipes3 = new GameObject("pipes1", glm::vec3(0, 0, 0));
	pipes3->setMesh(pipes3Mesh);
	pipes3->setMaterial(litMaterial);
	pipes3->setMeshRigidBody(_physicsWorld->GetDynamicsWorld()); //ONLY USE THE MESHRIGIDBODY FOR STATIC OBJECTS!
	_physicsWorld->add(pipes3);

	GameObject* pipes4 = new GameObject("pipes1", glm::vec3(0, 0, 0));
	pipes4->setMesh(pipes4Mesh);
	pipes4->setMaterial(litMaterial);
	pipes4->setMeshRigidBody(_physicsWorld->GetDynamicsWorld()); //ONLY USE THE MESHRIGIDBODY FOR STATIC OBJECTS!
	_physicsWorld->add(pipes4);
	/**/
	//FANS
	GameObject* airCond = new GameObject("airCond", glm::vec3(0, 0, 0));
	airCond->setMesh(airCondi);
	airCond->setMaterial(litMaterial1);
	_physicsWorld->add(airCond);

	GameObject* airCondGrid = new GameObject("airCondGrid", glm::vec3(0, 0, 0));
	airCondGrid->setMesh(airCondiGrid);
	airCondGrid->setMaterial(litMaterial1);
	_physicsWorld->add(airCondGrid);
	/**/

	//doors //Wonder if this will be deleted
	/**/
	GameObject* door1 = new GameObject("door", glm::vec3(-7, 1, -3));
	door1->setMesh(doorMesh);
	door1->setMaterial(litMaterial3);
	door1->setRigidBody(new btBoxShape(btVector3(0.5, 0.5f, 0.1f)), 0, _physicsWorld->GetDynamicsWorld());
	//door1->AddBehaviour(new DoorBehavior());
	_physicsWorld->add(door1);
	/**/

	/**/
	GameObject* oven = new GameObject("oven", glm::vec3(-35, 0, -15));
	oven->setMesh(ovenMesh);
	oven->setMaterial(ovenMaterial);
	oven->setRigidBody(new btBoxShape(btVector3(0.5, 0.5f, 1)), 0, _physicsWorld->GetDynamicsWorld());
	_physicsWorld->add(oven);
	/**/


	//PLAYER
	//todo:delete the bullet when hits the walll or anything else
	//TODO:make collisions
	GameObject* player = new GameObject("player", glm::vec3(-42, 1, -15));
	player->setMesh(playerMesh);//set mesh of player
	player->setMaterial(playerMaterial);//set material of player
	player->setRigidBody(new btSphereShape(1), 1, _physicsWorld->GetDynamicsWorld());//add collider and set as rigidbody //Don't delete this though. It should stay.
	player->AddBehaviour(new MovementBehaviour(_window, 100, 100));//add smovement behavior // You should probably delete this.
	player->AddBehaviour(new ShootingbehaviourPlayer(0.9f, bulletMesh, woodMaterial,_physicsWorld->GetDynamicsWorld(),_window));//add shooting behavior // And this.
	
	if (btRigidBody* rb = player->getRigidBody()->GetRB())
	{
		rb->setDamping(0.9999f, 1);
		rb->setAngularFactor(btVector3(0, 0, 0));

	}
	_physicsWorld->add(player);

	/**/
	//ENEMY
	GameObject* enemy = new GameObject("enemy", glm::vec3(-27, 1, 0));
	enemy->setMesh(enemiesMesh);
	enemy->setMaterial(enemy1Material);
	enemy->setRigidBody(new btSphereShape(0.5f), 1, _physicsWorld->GetDynamicsWorld());
	_physicsWorld->add(enemy);

	GameObject* enemy2 = new GameObject("enemy", glm::vec3(-8, 1, -10));
	enemy2->setMesh(enemiesMesh2);
	enemy2->setMaterial(enemy2Material);
	enemy2->setRigidBody(new btSphereShape(0.5f), 1, _physicsWorld->GetDynamicsWorld());
	_physicsWorld->add(enemy2);

	/**/
	//Todo:interactable objects
	//Todo:player die.

	//Done:trigger
	//trigger
	/**/
	Trigger* trigger = new Trigger("triggerInter", glm::vec3(-10, 1, 3));
	trigger->setMesh(barrelMesh1);
	trigger->setMaterial(woodMaterial);
	trigger->setRigidBody(new btSphereShape(0.5f), 0, _physicsWorld->GetDynamicsWorld());
	trigger->getRigidBody()->GetRB()->activate(true);
	trigger->getRigidBody()->GetRB()->setCollisionFlags(trigger->getRigidBody()->GetRB()->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	_physicsWorld->add(trigger);

	Trigger* trigger2 = new Trigger("triggerFeed1", glm::vec3(-25, 1, -15));
	trigger2->setMesh(barrelMesh1);
	trigger2->setMaterial(woodMaterial);
	trigger2->setRigidBody(new btSphereShape(0.5f), 0, _physicsWorld->GetDynamicsWorld());
	trigger2->getRigidBody()->GetRB()->activate(true);
	trigger2->getRigidBody()->GetRB()->setCollisionFlags(trigger->getRigidBody()->GetRB()->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	_physicsWorld->add(trigger2);

	Interactable* interact = new Interactable(door1,"interactable", glm::vec3(-10, 1, 0));
	interact->setMesh(interactableMesh);
	interact->setMaterial(woodMaterial);
	interact->setRigidBody(new btSphereShape(0.5f), 1, _physicsWorld->GetDynamicsWorld());
	if (btRigidBody* rb = interact->getRigidBody()->GetRB())
	{
		rb->setDamping(0.9999f, 1);
		rb->setAngularFactor(btVector3(0, 0, 0));
		
	}
	_physicsWorld->add(interact);
	/**/
	//todo:add feedback
	//sprite for movement
	//trigger that shows images
	//show that you can shoot
	//then show that there is interactable item


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//														LIGHTS || shadow											   					///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//pDirection is the point you want to lookAt for the moment
	DirectionalLight *dirLight1 = new DirectionalLight("Directional_Light_1", glm::vec3(0, 4, 0), glm::vec3(0.3f,0.3f, 0.3f), glm::vec3(0, -1,0));

	/**/
	//todo optimize shader
	//todo:make lights shine same

	/**/
	PointLight *pointLight2 = new PointLight("Point Light 3", glm::vec3(-35, 3, -13), glm::vec3(0.5f, 0.5f, 0));
	pointLight2->setMesh(bulletMesh);
	pointLight2->setMaterial(woodMaterial);
	_physicsWorld->add(pointLight2);
	/**/
	PointLight *pointLight3 = new PointLight("Point Light 1", glm::vec3(-27, 3, 0), glm::vec3(0.2f, 0, 0.2f));
	pointLight3->setMesh(bulletMesh);
	pointLight3->setMaterial(woodMaterial);
	_physicsWorld->add(pointLight3);
	/**/
	PointLight *pointLight4 = new PointLight("Point Light 1", glm::vec3(-8, 3, -10), glm::vec3(0, 0.5f, 0.5f));
	pointLight4->setMesh(bulletMesh);
	pointLight4->setMaterial(woodMaterial);
	_physicsWorld->add(pointLight4);
	/**/
	PointLight *pointLight5 = new PointLight("Point Light 1", glm::vec3(4, 3, -10), glm::vec3(0.5f, 0, 0.5f));
	pointLight5->setMesh(bulletMesh);
	pointLight5->setMaterial(woodMaterial);
	_physicsWorld->add(pointLight5);
	/**/
	//SpotLight *spotLight1 = new SpotLight("Spot Light 1", glm::vec3(-40, 10, -15), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0));
	//SpotLight *spotLight2 = new SpotLight("Spot Light 1", glm::vec3(-30, 10, -15), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0));
	//SpotLight *spotLight3 = new SpotLight("Spot Light 1", glm::vec3(-26, 10, 0), glm::vec3(0.5f, 0.1f, 0.1f), glm::vec3(0, -1, 0));

	//camera->AddBehaviour(new OrbCamera(5,-89,89,3, door1,_window));
	camera->AddBehaviour(new OrbitBehavior(15, player));
}

void KotrynaScene::_render()
{
	AbstractGame::_render();
	_updateHud();
}

void KotrynaScene::_update(float pStep)
{
	_physicsWorld->SimulateWorld(pStep);
	//cout << "Simulated the Dynamicsworld for " << pStep << " second" << endl;
	AbstractGame::_update(pStep);
	//_dynamicsWorld->debugDrawWorld();

}

void KotrynaScene::_updateHud()
{
	//ammo
	/**
	_hudAmmo->CreateAmmo();
	_hudAmmo->setTexture("123.png", 0, 800);
	_hudAmmo->draw();

	//meh.
	//TIMER
	string time = "";
	time += string("TIME Hi Max! and Hans...") + "\n";
	_hudTime->setUpFontColor(sf::Color::Yellow);
	_hudTime->setUpPosition(960, 1000);
	_hudTime->setDebugInfo(time);
	_hudTime->setTexture("unicorn.png", 960, 800);
	_hudTime->draw();

	if(_trig)//draw just when this crap is true
	{
		_walkableHUD->CreateFeedBack("move.png");
		_walkableHUD->draw();
	}/**/
	

	////FPS
	string debugInfo = "";
	debugInfo += string("FPS:") + std::to_string((int)_fps) + "\n";
	_hud->setUpPosition(0, 0);
	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

KotrynaScene::~KotrynaScene()
{

}