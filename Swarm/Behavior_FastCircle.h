////////////////////////////////////////////////////////////
// < Fichier : "Behavior_FastCircle.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef BEHAVIOR_FASTCIRCLE_H
#define Behavior_FastCircle_H
#include "Behavior.h"
#include "Swarm_child.h"

class Behavior_FastCircle :
	public Behavior
{
public:
	Behavior_FastCircle(Swarm_child* child);
	~Behavior_FastCircle();

	void	update(const sf::Time&);
private:
	void	calculate();
	Swarm_child*				m_child;
	sf::Vector2f				m_center;
	float						m_radius;
};

#endif
