#pragma once
#include "AI/Navigation/NavigationNode.h"
#include <vector>

class NavigationPath
{
public:
	NavigationPath();
	~NavigationPath();
	NavigationPath* AddNode(NavigationNode* node);
	int GetNodeCount() const;
	NavigationNode* GetNodeAt(int i);
private:
	std::vector<NavigationNode *>	_nodes;
};

