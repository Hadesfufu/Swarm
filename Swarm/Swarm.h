////////////////////////////////////////////////////////////
// < Fichier : "Swarm.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef SWARM_H
#define SWARM_H
#include <vector>
#include "Behavior_goToCenter.h"

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
	const Swarm_child* getChildAtPosition(const sf::Vector2f& pos) const;

	void  deleteChildAt(const sf::Vector2f& pos) const;
	void  addChildAt(const sf::Vector2f& pos);
private:
	
	std::vector< Swarm_child* > m_childen;
};

#endif
