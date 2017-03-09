#include "NavigationNode.h"
#include <ostream>
#include <iostream>

NavigationNode::NavigationNode(glm::vec3 position)
{
	_position = position;
}

NavigationNode::~NavigationNode()
{
}

glm::vec3 NavigationNode::getPosition() const
{
	return _position;
}

NavigationNode* NavigationNode::addNeighbours(NavigationNode* neighbour)
{
	std::cout << "Stringing nodes together!" << std::endl;
	_neigbours.push_back(neighbour);
	return this;
}

NavigationNode* NavigationNode::getRandomNeighbour()
{
	return _neigbours[rand() % _neigbours.size()];
}
