#include <iostream>
#include "mge/core/AbstractGame.hpp"
#include "mge/KotrynaScene.h"
#include "Physics/PhysicsWorldGame.h"

int main()
{
    std::cout << "Starting Game" << std::endl;
	//Physics Scene
	/**/
	AbstractGame* physicsScene = new PhysicsWorldGame();
	physicsScene->initialize();
	physicsScene->run();
	delete physicsScene;
	/**/


	/**
	//Kotryna Scene
	AbstractGame* kotrynaGame = new KotrynaScene();
	kotrynaGame->initialize();
	kotrynaGame->run();
	delete kotrynaGame;
    return 0;
	/**/
}



