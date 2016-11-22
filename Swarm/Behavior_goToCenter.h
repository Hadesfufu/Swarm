////////////////////////////////////////////////////////////
// < Fichier : "Behavior_goToCenter.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef BEHAVIOR_GOTOCENTER_H
#define BEHAVIOR_GOTOCENTER_H
#include "Behavior.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>


class Swarm_child;

class Behavior_goToCenter : public Behavior
{
public:
	Behavior_goToCenter(Swarm_child* child);
	~Behavior_goToCenter();

	void	update(const sf::Time&);
private:
	void calculate();
	Swarm_child*				m_child;
	sf::Vector2f				m_goal;
};


#endif 