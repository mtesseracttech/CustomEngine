#pragma once
#include "mge\core\AbstractGame.hpp"
#include "Physics/PhysicsWorld.h"
#include "mge/util/DebugHud.hpp"
#include "AI/Navigation/NavigationPath.h"

class PhysicsWorldGame : public AbstractGame
{
	public:
		PhysicsWorldGame();
		~PhysicsWorldGame();
		void initialize();
	protected:
		virtual void _initializeWorld();
		virtual void _initializeScene();
		virtual void _render();
		virtual void _update(float deltaTime);
	private:
		DebugHud*			_hud;
		PhysicsWorld*		_physicsWorld;
		void _updateHud();
		void HighlightNodes(NavigationPath* path1);
};

