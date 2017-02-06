#ifndef MARIJNSCENE_H
#define MARIJNSCENE_H

#include <mge/core/AbstractGame.hpp>

class DebugHud;

class MarijnScene : public AbstractGame
{
	//PUBLIC FUNCTIONS

public:
	MarijnScene();
	virtual ~MarijnScene();

	virtual void initialize();

protected:
	virtual void _initializeScene();

	//override render to render the hud as well.
	virtual void _render();

private:
	DebugHud* _hud;                   //hud display

	void _updateHud();

	MarijnScene(const MarijnScene&);
	MarijnScene& operator=(const MarijnScene&);
};

#endif // MARIJNSCENE_H
