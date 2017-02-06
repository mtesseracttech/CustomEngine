#ifndef KOTRYNASCENE_H
#define KOTRYNASCENE_H

#include <mge/core/AbstractGame.hpp>

class DebugHud;

class KotrynaScene : public AbstractGame
{
	//PUBLIC FUNCTIONS

public:
	KotrynaScene();
	virtual ~KotrynaScene();

	virtual void initialize();

protected:
	virtual void _initializeScene();

	//override render to render the hud as well.
	virtual void _render();

private:
	DebugHud* _hud;                   //hud display

	void _updateHud();

	KotrynaScene(const KotrynaScene&);
	KotrynaScene& operator=(const KotrynaScene&);
};

#endif // KOTRYNASCENE_H
