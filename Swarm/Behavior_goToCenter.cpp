#include "Behavior_goToCenter.h"
#include "Swarm.h"
#include "Swarm_child.h"
#include "Log.h"


Behavior_goToCenter::Behavior_goToCenter(Swarm_child* child) : m_child(child)
{
	calculate();
}


Behavior_goToCenter::~Behavior_goToCenter()
{
}

void Behavior_goToCenter::update(const sf::Time& dt)
{
	//Log::debug() << m_child->getId() << "a";
	m_child->moveTo(m_goal);
	//Log::debug() << m_child->getId() << "b";
}

void Behavior_goToCenter::calculate()
{
	const Swarm* swarm = m_child->getSwarm();
	for (auto it = swarm->getChildren().begin(); it != swarm->getChildren().end(); ++it){
		m_goal += (*it)->getPosition();
	}
	m_goal.x /= swarm->getChildren().size();
	m_goal.y /= swarm->getChildren().size();

}

