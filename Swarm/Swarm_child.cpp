#include "Swarm_child.h"
#include <ctime>
#include "Log.h"

int Swarm_child::m_currentId = 0;

Swarm_child::Swarm_child(Swarm* swarm) : m_thread_(&Swarm_child::update, this), m_swarm_(swarm), PhysBody(b2BodyType::b2_dynamicBody)
{
	m_speed = 50;
	float radius = 10;
	m_id = m_currentId;
	m_currentId++;
	m_shape = new sf::CircleShape(radius);
	setCircleHitbox(radius);
	m_shape->setFillColor(sf::Color(intRand(0, 256), intRand(0, 256), intRand(0, 256)));
	m_shape->setOutlineColor(sf::Color::White);
	m_shape->setOutlineThickness(1.f);
	setPosition(sf::Vector2f(intRand(0, 1280), intRand(0, 720)));
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
		sf::sleep(sf::milliseconds(100));
		if (m_Behavior)
			m_Behavior->update(m_clock.getElapsedTime());
		m_shape->setPosition(BtoSF<float>(PhysBody::getPositionB2()));
		m_shape->setRotation(PhysBody::getAngle() * 180 / b2_pi);
		if (getId() == 0)
			Log::debug() << m_shape->getPosition();
	}
}

void Swarm_child::stop()
{
	m_thread_.terminate();
}

void Swarm_child::moveTo(const sf::Vector2f& pos)
{
	sf::Vector2f currentPos = getPosition();
	if (currentPos == pos)
		return;
	sf::Vector2f actualDelta = pos - currentPos;
	float ratioX = actualDelta.x / (fabs(actualDelta.y) + fabs(actualDelta.x));
	float ratioY = actualDelta.y / (fabs(actualDelta.y) + fabs(actualDelta.x));
	sf::Vector2f newDelta(ratioX*m_speed, ratioY*m_speed);
	setMovement(SFtoB(newDelta));
}

int Swarm_child::intRand(const int& min, const int& max)
{
	static thread_local std::mt19937* generator = nullptr;
	if (!generator) generator = new std::mt19937(clock() + getId());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(*generator);
}

float Swarm_child::getDistanceFromOtherChild(const Swarm_child* child)
{
	sf::Vector2f newPos = getPosition() - child->getPosition();
	return std::sqrtf((newPos.x * newPos.x) + (newPos.y * newPos.y));
}

float Swarm_child::getNewDistanceFromOtherChild(const sf::Vector2f& input, const Swarm_child* child)
{
	sf::Vector2f newPos = input - child->getPosition();
	return std::sqrtf((newPos.x * newPos.x) + (newPos.y * newPos.y));	
}

void Swarm_child::setPosition(const sf::Vector2f& vec)
{
	m_shape->setPosition(vec);
	PhysBody::setPosition(SFtoB(vec));
}
