#include "ChaseState.h"
#include <ostream>
#include <iostream>

ChaseState::ChaseState(EnemyAgent* agent) : AbstractEnemyState(agent)
{
}

ChaseState::~ChaseState()
{
}

void ChaseState::Start()
{
	std::cout << "Starting " << typeid(*this).name() << "!" << std::endl;
}

void ChaseState::Stop()
{
	std::cout << "Stopping " << typeid(*this).name() << "!" << std::endl;
}

void ChaseState::Update(float deltaTime)
{
}
