////////////////////////////////////////////////////////////
// < Fichier : "Swarm.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef SWARM_H
#define SWARM_H
#include <vector>
#include "Behavior_goToCenter.h"

class Swarn_child;

class Swarm
{
public:
	Swarm();
	~Swarm();

	void createChild();
	void createChild(sf::Vector2f& vec);
	void createChild(int i);

	void goToCenter();
	void doACircle();
	void doALine();
	
	void update(sf::Time& dt);
	
	const std::vector< Swarm_child* >& getChildren() const { return  m_childen; }

private:
	
	std::vector< Swarm_child* > m_childen;
};

#endif
