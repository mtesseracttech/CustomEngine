#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "mge/core/AbstractGame.hpp"
#include "mge/KotrynaScene.h"

class SceneManager 
{
public:
	SceneManager();
	virtual ~ SceneManager();
	void initialize();
	void SetState(int state,bool restart = false);
	void NextLevel();

private:
	//KotrynaScene* _scene1;
	void StopState();
	void StartState();
	int _state;

};



#endif SCENEMANAHER_H
