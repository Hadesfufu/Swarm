#include "Swarm_child.h"
#include <ctime>
#include "Log.h"

Swarm_child::Swarm_child() : m_thread_(&Swarm_child::update, this)
{
	m_shape = new sf::CircleShape(10);
	m_shape->setFillColor(sf::Color(std::rand()%256, std::rand()%256, std::rand()%256));
	m_shape->setPosition(std::rand() % 1280, std::rand() % 720);
	m_thread_.launch();
}


Swarm_child::~Swarm_child()
{
	stop();
	if (m_shape)
		delete m_shape;
	if (m_Behavior)
		delete m_Behavior;
}

void Swarm_child::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_shape);
}

void Swarm_child::setBehavior(Behavior* bea)
{
	if (m_Behavior)
		delete m_Behavior;
	m_Behavior = bea;
}

void Swarm_child::update()
{
	while (true)
	{
		sf::sleep(sf::seconds(1));
		if (m_Behavior)
			m_Behavior->update();
	}
}

void Swarm_child::stop()
{
	m_thread_.terminate();
}