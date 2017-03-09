#pragma once
#include "LinearMath/btMotionState.h"
#include "mge/core/GameObject.hpp"

class GameObjectMotionState : public btMotionState
{
	public:
		GameObjectMotionState(const btTransform& initialPosition, GameObject* gameObject);
		~GameObjectMotionState();
		GameObject*		getGameObject()										const;
		void			getWorldTransform(btTransform& worldTrans)			const	override;
		void			setWorldTransform(const btTransform& worldTrans)	override;
	protected:
		GameObject*		_gameObject;
		btTransform		_initialPosition;
};

