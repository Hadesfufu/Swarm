#include "Swarm.h"
#include "Application.h"
#include "Drawer.h"
#include "Swarm_child.h"
#include "Behavior_FastCircle.h"
#include "Behavior_FastLine.h"


Swarm::Swarm()
{
}


Swarm::~Swarm()
{
	for (auto it = m_childen.begin(); it != m_childen.end(); ++it){
		if (*it)
			delete *it;
	}
	m_childen.clear();
}

void Swarm::createChild()
{
	m_childen.push_back(new Swarm_child(this));
	Drawer::I()->addToLayer(0, m_childen.back());
}

void Swarm::createChild(sf::Vector2f& vec)
{
	createChild();
	m_childen.back()->setPosition(vec);
}


void Swarm::createChild(int n)
{
	for(int i = 0; i < n; ++i){
		createChild();
    }
}

void Swarm::goToCenter(){
	for (auto it = m_childen.begin(); it != m_childen.end(); ++it){
		(*it)->setBehavior(new Behavior_goToCenter(*it));
	}
}

void Swarm::doACircle()
{
	for (auto it = m_childen.begin(); it != m_childen.end(); ++it){
		(*it)->setBehavior(new Behavior_FastCircle(*it));
	}
}

void Swarm::doALine()
{
	for (auto it = m_childen.begin(); it != m_childen.end(); ++it){
		(*it)->setBehavior(new Behavior_FastLine(*it));
	}		
}

void Swarm::update(sf::Time& dt)
{
	for (auto it = m_childen.begin(); it != m_childen.end(); ++it){
		(*it)->update(dt);
    }
}

const Swarm_child* Swarm::getChildAtPosition(const sf::Vector2f& pos) const
{
	for (auto it = m_childen.begin(); it != m_childen.end(); ++it)
	{
		if ((*it)->getDistanceFromPoint(pos) < (*it)->getRadius())
		{
			return (*it);
		}
	}
}

void Swarm::deleteChildAt(const sf::Vector2f& pos) const
{
	for (auto it = m_childen.begin(); it != m_childen.end(); ++it)
	{
		if ((*it)->getDistanceFromPoint(pos) < (*it)->getRadius())
		{
			delete *it;
			return;
		}
	}
}