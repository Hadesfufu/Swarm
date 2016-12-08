#include "Behavior_FastLine.h"

Behavior_FastLine::Behavior_FastLine(Swarm_child* child) : m_child(child)
{
	calculate();
}

Behavior_FastLine::~Behavior_FastLine()
{
}

void Behavior_FastLine::update(const sf::Time& dt)
{
	if (m_child->getPosition().y > m_latitude + 0.02 || m_child->getPosition().y < m_latitude - 0.02){
		if (m_child->getSpeed().y > -0.00002 && m_child->getSpeed().y < 0.00002){
			sf::Vector2f buff(m_child->getPosition().x * (std::rand() % 2 - 1), m_latitude);
			m_child->moveTo(buff);
		}
		else
			m_child->moveTo(sf::Vector2f(m_child->getPosition().x, m_latitude));
	}
	else
	{
		Swarm_child* a = m_child->getNearestChild();
		Swarm_child* b = m_child->getNearestChild(a);
		float da = m_child->getDistanceFromOtherChild(a);
		float db = m_child->getDistanceFromOtherChild(b);
		float d = db - da;
		if (d > 1 || d < -1){
			m_child->moveAway(a->getPosition(), d/2);
		}
	}
}


void Behavior_FastLine::calculate()
{
	const Swarm* swarm = m_child->getSwarm();
	unsigned int nbChild = swarm->getChildren().size();
	for (auto it = swarm->getChildren().begin(); it != swarm->getChildren().end(); ++it){
		m_latitude += (*it)->getPosition().y;
	}
	m_latitude /= nbChild;
}