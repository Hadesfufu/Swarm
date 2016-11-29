#include "Swarm_child.h"
#include <ctime>
#include "Log.h"

int Swarm_child::m_currentId = 0;

Swarm_child::Swarm_child(Swarm* swarm) :  m_swarm_(swarm), PhysBody(b2BodyType::b2_dynamicBody)
{
	m_speed = 5;
	float radius = 10;
	m_id = m_currentId;
	m_currentId++;
	m_shape = new sf::CircleShape(radius);
	setCircleHitbox(radius+1.f);
	m_shape->setFillColor(sf::Color(intRand(0, 256), intRand(0, 256), intRand(0, 256)));
	m_shape->setOutlineColor(sf::Color::White);
	m_shape->setOutlineThickness(1.f);
	setPosition(sf::Vector2f(intRand(0, 1280), intRand(0, 720)));
}


Swarm_child::~Swarm_child()
{
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

void Swarm_child::update(sf::Time&dt)
{
	if (m_Behavior)
		m_Behavior->update(dt);
	m_shape->setPosition(BtoSF<float>(PhysBody::getPositionB2()));
	m_shape->setRotation(PhysBody::getAngle() * 180 / b2_pi);
	if (getId() == 0)
	{
	}
}

void Swarm_child::moveTo(const sf::Vector2f& pos)
{
	sf::Vector2f currentPos = getPosition();
	if (currentPos == pos)
		return;
	sf::Vector2f actualDelta = pos - currentPos;
	float ratioX, ratioY;
	ratioX = actualDelta.x / (fabs(actualDelta.y) + fabs(actualDelta.x));
	ratioY = actualDelta.y / (fabs(actualDelta.y) + fabs(actualDelta.x));

	sf::Vector2f newDelta(ratioX*m_speed, ratioY*m_speed);
	if (fabs(actualDelta.x) < fabs(newDelta.x))
		newDelta.x = actualDelta.x;
	if (fabs(actualDelta.y) < fabs(newDelta.y))
		newDelta.y = actualDelta.y;
	newDelta -= BtoSF<float>(PhysBody::getSpeed());
	applyImpulse(SFtoB(newDelta));
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
float Swarm_child::getDistanceFromPoint(const sf::Vector2f& vec){
	sf::Vector2f newPos = getPosition() - vec;
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
