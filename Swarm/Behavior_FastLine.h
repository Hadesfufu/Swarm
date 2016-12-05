////////////////////////////////////////////////////////////
// < Fichier : "Behavior_FastLine.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef BEHAVIOR_FASTLINE
#define BEHAVIOR_FASTLINE
#include "Behavior.h"
#include "Swarm_child.h"

class Behavior_FastLine : public Behavior
{
public:
	Behavior_FastLine(Swarm_child* child);
	~Behavior_FastLine();

	void	update(const sf::Time&);
private:
	void	calculate();
	Swarm_child*				m_child;
	float						m_latitude;
};
#endif

