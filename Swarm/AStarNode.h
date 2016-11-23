#ifndef ASTARNODE_H
#define ASTARNODE_H
#include <SFML/System/Vector2.hpp>

class AStarNode
{
public:
	AStarNode(sf::Vector2f& pos, int l, int p);
	AStarNode(sf::Vector2f& pos, int l, int p);
	~AStarNode();
private:
	sf::Vector2f currentPosition;
	int level;
	int priority;
};

#endif
