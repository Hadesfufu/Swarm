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
	m_child->moveTo(m_center);
	//Log::debug() << m_child->getId() << "b";
}

void Behavior_FastCircle::calculate()
{
	const Swarm* swarm = m_child->getSwarm();
	unsigned int nbChild = swarm->getChildren().size();
	for (auto it = swarm->getChildren().begin(); it != swarm->getChildren().end(); ++it){
		m_center += (*it)->getPosition();
	}
	m_center.x /= nbChild;
	m_center.y /= nbChild;

	for (auto it = swarm->getChildren().begin(); it != swarm->getChildren().end(); ++it){
		m_radius += (*it)->getDistanceFromPoint(m_center);
	}
	m_radius /= nbChild;
}

