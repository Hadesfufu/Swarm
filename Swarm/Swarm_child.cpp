#include "Swarm_child.h"
#include <ctime>
#include "Log.h"

int Swarm_child::m_currentId = 0;

Swarm_child::Swarm_child(Swarm* swarm) : m_thread_(&Swarm_child::update, this), m_swarm_(swarm)
{
	m_id = m_currentId;
	m_currentId++;
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
		m_clock.restart();
		sf::sleep(sf::seconds(1/60));
		if (m_Behavior)
			m_Behavior->update();
	}
}

void Swarm_child::stop()
{
	m_thread_.terminate();
}

void Swarm_child::moveTo(const sf::Vector2f& pos)
{
	if (getPosition() == pos)
		return;
	sf::Vector2f actualDelta = pos - getPosition();
	float ratio = pos.x / pos.y;
	float speed = m_speed*m_clock.getElapsedTime().asSeconds();
	sf::Vector2f newDelta(ratio*speed, (1 - ratio)*speed);
	sf::Vector2f newPos = newDelta + getPosition();
	if (fabs(newDelta.x) > fabs(actualDelta.x))
		newPos.x = pos.x;
	if (fabs(newDelta.y) > fabs(actualDelta.y))
		newPos.y = pos.y;
	setPosition(newPos);
}