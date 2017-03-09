#include "LookoutState.h"
#include "AttackState.h"
#include <iostream>

LookoutState::LookoutState(EnemyAgent* agent) : AbstractEnemyState(agent)
{
}

LookoutState::~LookoutState()
{
}

void LookoutState::Start()
{
	std::cout << "Starting " << typeid(*this).name() << "!" << std::endl;
}

void LookoutState::Stop()
{
	std::cout << "Stopping " << typeid(*this).name() << "!" << std::endl;
}

void LookoutState::Update(float deltaTime)
{
}
