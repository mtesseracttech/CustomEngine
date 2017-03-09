#include "SceneManager.h"


SceneManager::SceneManager()
{
	SetState(1);
}

SceneManager::~SceneManager()
{
	
}

void SceneManager::NextLevel()
{
	
}

void SceneManager::SetState(int state, bool restart)
{
	if (state == _state && !restart)return;
	StopState();
	_state = state;
	StartState();
}

void SceneManager::StartState()
{
	switch (1)
	{
	case 1:
		
		break;
	default: break;
	}
}

void SceneManager::StopState()
{
	switch (1)
	{
	case 1:
		//delete _scene1;
		break;
	default: break;
	}
}






