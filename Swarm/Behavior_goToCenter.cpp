#include "Behavior_goToCenter.h"
#include "Swarm.h"
#include "Swarm_child.h"


Behavior_goToCenter::Behavior_goToCenter(Swarm_child* child) : m_child(child)
{
}


Behavior_goToCenter::~Behavior_goToCenter()
{
}

void Behavior_goToCenter::update()
{
	
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

