#pragma once
#include "mge/core/AbstractGame.hpp"
#include "mge/util/DebugHud.hpp"
#include "Physics/PhysicsWorld.h"
#include "audio/MusicPlayer.hpp"


class KotrynaScene : public AbstractGame
{
public:
	KotrynaScene();
	virtual ~KotrynaScene();
	void initialize();
	
protected:
	virtual void _initializeScene();
	virtual void _initializeWorld();
	virtual void _render();
	virtual void _update(float pStep);

private:
	//DebugHud*			_walkableHUD;
	//DebugHud*			_shootingHUD;
	DebugHud*			_hud;
	//DebugHud*			_hudTime;
	//DebugHud*			_hudAmmo;
//	DebugHud*			_hudFeedback;
	PhysicsWorld*		_physicsWorld;
	MusicPlayer _musicPlayer;
	void _updateHud();
	bool _trig = false;

};
