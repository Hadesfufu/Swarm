#include "Swarm.h"
#include "Application.h"
#include "Drawer.h"
#include "Swarm_child.h"


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
	m_childen.emplace_back(new Swarm_child(this));
	Drawer::I()->addToLayer(0, m_childen.back());
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

void Swarm::update(sf::Time& dt)
{
	for (auto it = m_childen.begin(); it != m_childen.end(); ++it){
		(*it)->update(dt);
    }
}
