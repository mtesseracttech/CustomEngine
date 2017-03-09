#pragma once
#include <glm/detail/type_vec3.hpp>
#include <vector>

class NavigationNode
{
public:
	NavigationNode(glm::vec3 position);
	~NavigationNode();
	glm::vec3						getPosition() const;
	NavigationNode* addNeighbours(NavigationNode* neighbour);
	NavigationNode* getRandomNeighbour();
private:
	glm::vec3						_position;
	std::vector<NavigationNode *>	_neigbours;
};

