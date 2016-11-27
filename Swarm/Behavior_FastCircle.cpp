#include "Behavior_FastCircle.h"


Behavior_FastCircle::Behavior_FastCircle(Swarm_child* child): m_child(child)
{
	calculate();
}


Behavior_FastCircle::~Behavior_FastCircle()
{
}


void Behavior_FastCircle::update(const sf::Time& dt)
{
	//Log::debug() << m_child->getId() << "a";
	m_child->moveTo(m_goal);
	//Log::debug() << m_child->getId() << "b";
}

void Behavior_FastCircle::calculate()
{
	const Swarm* swarm = m_child->getSwarm();
	for (auto it = swarm->getChildren().begin(); it != swarm->getChildren().end(); ++it){
		m_goal += (*it)->getPosition();
	}
	m_goal.x /= swarm->getChildren().size();
	m_goal.y /= swarm->getChildren().size();
}

