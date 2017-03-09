#include "NavigationPath.h"
#include <iostream>


NavigationPath::NavigationPath()
{
}


NavigationPath::~NavigationPath()
{
}

NavigationPath* NavigationPath::AddNode(NavigationNode* node)
{
	std::cout << "adding node!" << std::endl;
	_nodes.push_back(node);
	return this;
}


int NavigationPath::GetNodeCount() const
{
	return _nodes.size();
}

NavigationNode* NavigationPath::GetNodeAt(int i)
{
	return _nodes[i];
}
