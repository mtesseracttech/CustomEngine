#ifndef BARTSCENE_H
#define BARTSCENE_H

#include <mge/core/AbstractGame.hpp>

class DebugHud;

class BartScene : public AbstractGame
{
	//PUBLIC FUNCTIONS

public:
	BartScene();
	virtual ~BartScene();

	virtual void initialize();

protected:
	virtual void _initializeScene();

	//override render to render the hud as well.
	virtual void _render();

private:
	DebugHud* _hud;                   //hud display

	void _updateHud();

	BartScene(const BartScene&);
	BartScene& operator=(const BartScene&);
};

#endif // MARIJNSCENE_H
