#include "PhysBody.h"
#include <limits>
#include <queue>

b2World*			PhysBody::c_World;
const float32		PhysBody::c_BodyScale= 30.f;

////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

PhysBody::PhysBody(b2BodyType bt) : m_originType(TopLeftCorner){
	m_type = &typeid(PhysBody);
	if (!c_World){
		Log::error("PhysBody") << "You need to setup the world first";
		throw "You need to setup the world first";
	}
	m_size.x = 0;
	m_size.y = 0;
	b2BodyDef def;
	def.type = bt;
	def.fixedRotation = true;
	def.userData = this;
	m_body = c_World->CreateBody(&def);
	if (!m_body)
		Log::error("Body Failed the Creation");
}

////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////
PhysBody::~PhysBody()
{
	if (m_body && c_World && m_body != (const b2Body*)0xFDFDFDFD)
		c_World->DestroyBody(m_body);
}

////////////////////////////////////////////////////////////
// Setters
////////////////////////////////////////////////////////////
void PhysBody::setFixtureData(float friction, float density, float restitution){
	setFixtureDensity(density);
	setFixtureFriction(friction);
	setFixtureRestitution(restitution);
}

////////////////////////////////////////////////////////////
// Density
////////////////////////////////////////////////////////////

void PhysBody::setFixtureDensity(float density){
	if (!m_body->GetFixtureList()){
		Log::warning("PhysBody - Fixture") << "No Fixture";
		return;
	}
	b2Fixture* fix = m_body->GetFixtureList();
	while (fix){
		fix->SetDensity(density);
		fix = fix->GetNext();
	}
	m_body->ResetMassData();
}

////////////////////////////////////////////////////////////
// Friction
////////////////////////////////////////////////////////////

void PhysBody::setFixtureFriction(float friction){
	if (!m_body->GetFixtureList()){
		Log::warning("PhysBody - Fixture") << "No Fixture";
		return;
	}
	b2Fixture* fix = m_body->GetFixtureList();
	while (fix){
		fix->SetFriction(friction);
		fix = fix->GetNext();
	}
}

////////////////////////////////////////////////////////////
// Restitution
////////////////////////////////////////////////////////////


void PhysBody::setFixtureRestitution(float restitution){
	if (!m_body->GetFixtureList()){
		Log::warning("PhysBody - Fixture") << "No Fixture";
		return;
	}
	b2Fixture* fix = m_body->GetFixtureList();
	while (fix){
		fix->SetRestitution(restitution);
		fix = fix->GetNext();
	}
}

void PhysBody::setFixtureSensor(bool sensor){
	if (!m_body->GetFixtureList()){
		Log::warning("PhysBody - Fixture") << "No Fixture";
		return;
	}
	b2Fixture* fix = m_body->GetFixtureList();
	while (fix){
		fix->SetSensor(sensor);
		fix = fix->GetNext();
	}
}
////////////////////////////////////////////////////////////
// Filtering Data
////////////////////////////////////////////////////////////

void	PhysBody::setFixtureFilterData(const b2Filter& filter){
	if (!m_body->GetFixtureList()){
		Log::warning("PhysBody - Fixture") << "No Fixture";
		return;
	}
	b2Fixture* fix = m_body->GetFixtureList();
	while (fix){
		fix->SetFilterData(filter);
		fix = fix->GetNext();
	}
}

void	PhysBody::setFixtureCollisionMask(CollisionGroup cg){
	if (!m_body->GetFixtureList()){
		Log::warning("PhysBody - Fixture") << "No Fixture";
		return;
	}
	b2Fixture* fix = m_body->GetFixtureList();
	while (fix){
		b2Filter filter = fix->GetFilterData();
		filter.maskBits = cg;
		fix->SetFilterData(filter);
		fix = fix->GetNext();
	}
}

void	PhysBody::addFixtureCollisionMask(CollisionGroup cg){
	if (!m_body->GetFixtureList()){
		Log::warning("PhysBody - Fixture") << "No Fixture";
		return;
	}
	b2Fixture* fix = m_body->GetFixtureList();
	while (fix){
		b2Filter filter = fix->GetFilterData();
		if ((cg & filter.maskBits) == 0)
			filter.maskBits = filter.maskBits | cg;
		fix->SetFilterData(filter);
		fix = fix->GetNext();
	}
}

void	PhysBody::removeFixtureCollisionMask(CollisionGroup cg){
	if (!m_body->GetFixtureList()){
		Log::warning("PhysBody - Fixture") << "No Fixture";
		return;
	}
	b2Fixture* fix = m_body->GetFixtureList();
	while (fix){
		b2Filter filter = m_body->GetFixtureList()->GetFilterData();
		if ((cg & filter.maskBits) == 0){
			filter.maskBits = filter.maskBits ^ cg;
		}
		m_body->GetFixtureList()->SetFilterData(filter);
		fix = fix->GetNext();
	}
}

void	PhysBody::setFixtureCategoryMask(CollisionGroup cg){
	if (!m_body->GetFixtureList()){
		Log::warning("PhysBody - Fixture") << "No Fixture";
		return;
	}
	b2Fixture* fix = m_body->GetFixtureList();
	while (fix){
		b2Filter filter = fix->GetFilterData();
		filter.categoryBits = cg;
		fix->SetFilterData(filter);
		fix = fix->GetNext();
	}
}


////////////////////////////////////////////////////////////
// Position
////////////////////////////////////////////////////////////

void PhysBody::setPosition(b2Vec2 pos){
	pos.Set(pos.x / c_BodyScale, pos.y / c_BodyScale);
	m_body->SetTransform(pos, m_body->GetAngle());
	setMovement(b2Vec2(0, 0));
}

////////////////////////////////////////////////////////////
// Movement
////////////////////////////////////////////////////////////

void PhysBody::setMovement(const b2Vec2& speed){
	m_body->SetLinearVelocity(speed);
}

////////////////////////////////////////////////////////////
// MvX
////////////////////////////////////////////////////////////

void		 PhysBody::setMovementX(float32 speed){
	m_body->SetLinearVelocity(b2Vec2(speed, m_body->GetLinearVelocity().y));
}

////////////////////////////////////////////////////////////
// MvY
////////////////////////////////////////////////////////////

void		 PhysBody::setMovementY(float32 speed){
	m_body->SetLinearVelocity(b2Vec2(m_body->GetLinearVelocity().x, speed));
}

////////////////////////////////////////////////////////////
// Origin
////////////////////////////////////////////////////////////

void		 PhysBody::setOriginType(OriginType ot){
	m_originType = ot;
}

void		 PhysBody::setOriginType(std::string& str){
	if (str == "Center")
		m_originType = Center;
	else if (str == "Left")
		m_originType = Left;
	else if (str == "Right")
		m_originType = Right;
	else if (str == "Top")
		m_originType = Top;
	else if (str == "Bot")
		m_originType = Bot;
	else if (str == "TopLeftCorner")
		m_originType = TopLeftCorner;
	else if (str == "TopRightCorner")
		m_originType = TopRightCorner;
	else if (str == "BotLeftCorner")
		m_originType = BotLeftCorner;
	else if (str == "BotRightCorner")
		m_originType = BotRightCorner;
	else
		Log::error("PhysBody - setOrigin") << "Wrong input";
}
////////////////////////////////////////////////////////////
// Hitbox
////////////////////////////////////////////////////////////

void PhysBody::setHitbox(const b2Vec2& size){
	b2Fixture*   currentFixture = m_body->GetFixtureList();	
	b2PolygonShape Box;

	// box created with half heihgt & width from origin
	Box.SetAsBox(size.x / (2 * c_BodyScale), size.y / (2 * c_BodyScale), getOriginWorker(size),0.f);
	m_size = size;

	b2FixtureDef fixturedef;
	if (currentFixture){
		fixturedef.density = currentFixture->GetDensity();
		fixturedef.restitution = currentFixture->GetRestitution();	
		fixturedef.friction = currentFixture->GetFriction();
		fixturedef.filter = currentFixture->GetFilterData();
		m_body->DestroyFixture(currentFixture);
	}
	fixturedef.shape = &Box;
	m_body->CreateFixture(&fixturedef);
}

void PhysBody::setHitbox(std::vector<b2Vec2>& segments){
	b2Fixture*   currentFixture = m_body->GetFixtureList();
	b2PolygonShape* Box = new b2PolygonShape();
	b2Fixture*	 debug = (b2Fixture*)(0xFDFDFDFD);

	for (auto it = segments.begin(); it != segments.end(); ++it){
		m_size.x = std::max(it->x, m_size.x);
		m_size.y = std::max(it->y, m_size.y);
    }

	// box created with half height & width from origin
	Box->Set(&segments[0], segments.size());

	b2FixtureDef fixturedef;
	if (currentFixture && currentFixture != debug){
		fixturedef.density = currentFixture->GetDensity();
		fixturedef.restitution = currentFixture->GetRestitution();
		fixturedef.friction = currentFixture->GetFriction();
		fixturedef.filter = currentFixture->GetFilterData();
		m_body->DestroyFixture(currentFixture);
	}

	fixturedef.shape = Box;
	m_body->CreateFixture(&fixturedef);
	delete Box;
	m_body->ResetMassData();
}

void	PhysBody::setHitbox(std::vector<std::vector<b2Vec2>>& segments){
	b2FixtureDef fixturedef;
	b2Fixture*   currentFixture = m_body->GetFixtureList();
	b2Fixture*	 debug  = (b2Fixture*)(0xFDFDFDFD);

	if (currentFixture && currentFixture != debug){
		fixturedef.density = currentFixture->GetDensity();
		fixturedef.restitution = currentFixture->GetRestitution();
		fixturedef.friction = currentFixture->GetFriction(); 
		fixturedef.filter = currentFixture->GetFilterData();
		m_body->DestroyFixture(currentFixture);
	}
	for (auto it = segments.begin(); it != segments.end();++it)
	{
		b2PolygonShape Box;
		for(unsigned int i = 0; i < (*it).size(); ++i)
		{
			m_size.x = std::max((*it)[i].x, m_size.x);
			m_size.y = std::max((*it)[i].y, m_size.y);
		}

		Box.Set(&(*it)[0], (*it).size());
		fixturedef.shape = &Box;
		m_body->CreateFixture(&fixturedef);
    }
	m_body->ResetMassData();
}

void PhysBody::setCircleHitbox(float radius)
{
	b2FixtureDef fixturedef;
	b2Fixture*   currentFixture = m_body->GetFixtureList();
	b2Fixture*	 debug = (b2Fixture*)(0xFDFDFDFD);
	if (currentFixture && currentFixture != debug){
		fixturedef.density = currentFixture->GetDensity();
		fixturedef.restitution = currentFixture->GetRestitution();
		fixturedef.friction = currentFixture->GetFriction();
		fixturedef.filter = currentFixture->GetFilterData();
		m_body->DestroyFixture(currentFixture);	
	}
	b2CircleShape shape;
	shape.m_radius = radius/c_BodyScale;
	fixturedef.shape = &shape;
	m_body->CreateFixture(&fixturedef);
	
}


////////////////////////////////////////////////////////////
// BodyType
////////////////////////////////////////////////////////////


void PhysBody::setBodyType(b2BodyType type){
	if (!m_body){
		Log::warning("PhysBody - setBodyType") << "No Body is created";
		return;
	}
	m_body->SetType(type);
}
////////////////////////////////////////////////////////////

void PhysBody::setBodyRotation(bool rotation){
	if (!m_body){
		Log::warning("PhysBody - setBodyType") << "No Body is created";
		return;
	}
	m_body->SetFixedRotation(!rotation);
}

////////////////////////////////////////////////////////////
// World
////////////////////////////////////////////////////////////

void PhysBody::setWorld(b2World* world){
	c_World = world;
}


////////////////////////////////////////////////////////////
// Getters
////////////////////////////////////////////////////////////

float* PhysBody::getData() const{
	m_body->GetFixtureList()->GetDensity();
	m_body->GetFixtureList()->GetFriction();
	m_body->GetFixtureList()->GetRestitution();
	float *buff = new float[3]{
		m_body->GetFixtureList()->GetDensity(),
		m_body->GetFixtureList()->GetFriction(),
		m_body->GetFixtureList()->GetRestitution()
	};
	return buff;
}
////////////////////////////////////////////////////////////

b2Vec2 PhysBody::getPositionB2() const{
	b2Vec2 buff = m_body->GetPosition();
	buff *= c_BodyScale;
	return buff;
}
////////////////////////////////////////////////////////////

const b2Vec2& PhysBody::getSpeed() const{
	return m_body->GetLinearVelocity();
}
////////////////////////////////////////////////////////////

const b2Vec2& PhysBody::getHitBoxB2() const{
	return m_size;
}
////////////////////////////////////////////////////////////

float PhysBody::getAngle() const{
	return m_body->GetAngle();
}
////////////////////////////////////////////////////////////

const float PhysBody::getScale(){
	return c_BodyScale;
}
////////////////////////////////////////////////////////////

const b2World* PhysBody::getWorld(){
	return c_World;
}

////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////


void PhysBody::applyImpulse(const b2Vec2& force){
	m_body->ApplyLinearImpulse(force, m_body->GetLocalCenter(), true);
}

b2Vec2 PhysBody::getOriginWorker(const b2Vec2& size){
	b2Vec2 returner;
	switch (m_originType)
	{
	case Left:
		returner.x = 0;
		returner.y = size.y / (2 * c_BodyScale) ;
		break;
	case Right:
		returner.x = size.x / c_BodyScale;
		returner.y = size.y / (2 * c_BodyScale);
		break;
	case Top:
		returner.x = size.x / (2 * c_BodyScale);
		returner.y = 0;
		break;
	case Bot:
		returner.x = size.x / (2 * c_BodyScale);
		returner.y = size.y / c_BodyScale;
		break;
	case TopRightCorner:
		returner.x = size.x / c_BodyScale;
		returner.y = 0;
		break;
	case TopLeftCorner:
		returner.x = 0;
		returner.y = 0;
		break;
	case BotRightCorner:
		returner.x = size.x / c_BodyScale;
		returner.y = size.y / c_BodyScale;
		break;
	case BotLeftCorner:
		returner.x = 0;
		returner.y = size.y / c_BodyScale;
		break;
	case Center:
		returner.x = size.x / (2 * c_BodyScale);
		returner.y = size.y / (2 * c_BodyScale);
		break; 
	default:
		returner.x = 0;
		returner.y = 0;
		break;
	}
	returner.x /= c_BodyScale;
	returner.y /= c_BodyScale;
	/*returner.x = 0.5;
	returner.y = 0.5;*/
	return returner;
}