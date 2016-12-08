#include "Swarm_child.h"
#include <ctime>
#include "Log.h"

int Swarm_child::m_CurrentId = 0;

const sf::Vector2f& Min(const sf::Vector2f& a, const sf::Vector2f& b)
{
	float da = sqrt((a.x * a.x) + (a.y * a.y));
	float db = sqrt((b.x * b.x) + (b.y * b.y));
	if (da < db)
		return a;
	else
		return b;
}

Swarm_child::Swarm_child(Swarm* swarm) : m_Swarm(swarm), PhysBody(b2BodyType::b2_dynamicBody){
	m_Speed = 5;
	m_Radius = 10;
	m_Id = m_CurrentId;
	m_CurrentId++;
	m_Shape = new sf::CircleShape(m_Radius);
	setCircleHitbox(m_Radius + 1.f);
	m_Shape->setFillColor(sf::Color(intRand(0, 256), intRand(0, 256), intRand(0, 256)));
	m_Shape->setOutlineColor(sf::Color::White);
	m_Shape->setOutlineThickness(1.f);
	setPosition(sf::Vector2f(intRand(0, 1280), intRand(0, 720)));
}


Swarm_child::~Swarm_child()
{
	if (m_Shape)
		delete m_Shape;
	if (m_Behavior)
		delete m_Behavior;
}

void Swarm_child::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Shape);
}

void Swarm_child::update(sf::Time&dt)
{
	if (m_Behavior)
		m_Behavior->update(dt);
	m_Shape->setPosition(BtoSF<float>(PhysBody::getPositionB2()));
	m_Shape->setRotation(PhysBody::getAngle() * 180 / b2_pi);
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
	sf::Vector2f ratio = getRatio(pos);

	sf::Vector2f newDelta(ratio.x*m_Speed, ratio.y*m_Speed);
	if (fabs(actualDelta.x) < fabs(newDelta.x))
		newDelta.x = actualDelta.x;
	if (fabs(actualDelta.y) < fabs(newDelta.y))
		newDelta.y = actualDelta.y;
	newDelta -= BtoSF<float>(PhysBody::getSpeed());
	applyImpulse(SFtoB(newDelta));
}

void Swarm_child::moveAway(const sf::Vector2f& pos, float maxspeed)
{
	sf::Vector2f currentPos = getPosition();
	if (currentPos == pos)
		return;
	sf::Vector2f actualDelta = pos - currentPos;
	sf::Vector2f ratio = getRatio(pos);
	sf::Vector2f newDelta;
	float speed = std::fmin(maxspeed, m_Speed);
	newDelta.x = ratio.x*speed;
	newDelta.y = ratio.y*speed;
	newDelta = -newDelta;
	newDelta -= BtoSF<float>(PhysBody::getSpeed());
	applyImpulse(SFtoB(newDelta));
}


void Swarm_child::moveDir(const sf::Vector2f& ratio)
{
	sf::Vector2f currentPos = getPosition();
	sf::Vector2f newDelta(ratio.x*m_Speed, ratio.y*m_Speed);
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
	m_Shape->setPosition(vec);
	PhysBody::setPosition(SFtoB(vec));
}

void Swarm_child::setBehavior(Behavior* be)
{
	if (be)
	{
		if (m_Behavior)
			delete m_Behavior;
		else
			m_Behavior = be;
	}
	else
		Log::error("SwarmChild::SetBehavior") << "Bad Behavior";
}

sf::Vector2f Swarm_child::getRatio(const sf::Vector2f& goal)
{
	sf::Vector2f actualDelta = goal - getPosition();
	sf::Vector2f ratio;
	ratio.x = actualDelta.x / (fabs(actualDelta.y) + fabs(actualDelta.x));
	ratio.y = actualDelta.y / (fabs(actualDelta.y) + fabs(actualDelta.x));
	return ratio;
}

Swarm_child* Swarm_child::getNearestChild(Swarm_child* child)
{
	sf::Vector2f	dist(100000.f, 100000.f), newone;
	Swarm_child* buffer = nullptr;
	for (auto it = m_Swarm->getChildren().begin(); it != m_Swarm->getChildren().end(); ++it)
	{	
		if (*it != child && *it != this ){
			newone = Min(dist, (*it)->getPosition() - getPosition());
			if (dist != newone){
				dist = newone;
				buffer = *it;
			}
		}
	}
	return buffer;
}
