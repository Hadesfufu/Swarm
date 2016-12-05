#include "Behavior_FastCircle.h"
#include "Application.h"

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
	if (m_child->getDistanceFromPoint(m_nearestPointToGo) > 2){
		if (m_child->getSpeed().y > -0.00002 && m_child->getSpeed().y < 0.00002 && m_child->getSpeed().x > -0.00002 && m_child->getSpeed().x < 0.00002){
			sf::Vector2f buff(m_nearestPointToGo.x + (m_nearestPointToGo.x * (std::rand() % 2 - 1)), m_nearestPointToGo.y + (m_nearestPointToGo.y * (std::rand() % 2 - 1)));
			m_child->moveTo(buff);
		}
		else
			m_child->moveTo(m_nearestPointToGo);
	}
	else
	{
		Swarm_child* a = m_child->getNearestChild();
		Swarm_child* b = m_child->getNearestChild(a);
		float da = m_child->getDistanceFromOtherChild(a);
		float db = m_child->getDistanceFromOtherChild(b);
		float d = da - db;
		if (d < -1 && d > 1){
			m_child->moveAway(a->getPosition(),fabs(d));
		}
	}
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
	if (m_radius >= Application::I()->getWindowSize().y / 2)
		m_radius = Application::I()->getWindowSize().y / 2;

	float radSqr = m_radius*m_radius;
	sf::Vector2f ratio = m_child->getRatio(m_center);
	m_nearestPointToGo.x = sqrtf(fabs(ratio.x) * radSqr);
	m_nearestPointToGo.y = sqrtf(fabs(ratio.y) * radSqr);
	if (ratio.x >= 0)
		m_nearestPointToGo.x = m_center.x - m_nearestPointToGo.x;
	else
		m_nearestPointToGo.x = m_center.x + m_nearestPointToGo.x;

	if (ratio.y >= 0)
		m_nearestPointToGo.y = m_center.y - m_nearestPointToGo.y;
	else
		m_nearestPointToGo.y = m_center.y + m_nearestPointToGo.y;
}

