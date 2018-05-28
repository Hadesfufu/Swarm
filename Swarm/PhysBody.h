#ifndef PHYSBODY_H
#define PHYSBODY_H

#include "Box2D\Box2D.h"
#include "Log.h"
class PhysBody
{
public:
	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////
	PhysBody(b2BodyType = b2BodyType::b2_staticBody);

	////////////////////////////////////////////////////////////
	// Destructors
	////////////////////////////////////////////////////////////
	~PhysBody();

	////////////////////////////////////////////////////////////
	// Collision Groups
	////////////////////////////////////////////////////////////
	enum CollisionGroup{
		None = 0,
		MapElement = 1,
		Entity = 1 << 1,
		Particule = 1 << 2
	};


	////////////////////////////////////////////////////////////
	//	Note that the default origin type is TopLeftCorner
	////////////////////////////////////////////////////////////

	enum OriginType{
		Center,
		Left,
		Right,
		Top,
		Bot,
		TopLeftCorner,
		TopRightCorner,
		BotLeftCorner,
		BotRightCorner
	};

	////////////////////////////////////////////////////////////
	// 1 Setters
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	// 1.1 Movement elements
	////////////////////////////////////////////////////////////
	void							setPosition(b2Vec2 pos);
	void							setHitbox(const b2Vec2& size);
	void							setHitbox(std::vector<b2Vec2>& segments);
	void							setHitbox(std::vector<std::vector<b2Vec2>>& segments);
	void							setCircleHitbox(float radius);
	void							setMovement(const b2Vec2& speed);
	void							setMovementX(float32 speed);
	void							setMovementY(float32 speed);
	/*
		Use before setHitbox !
		Only works with SquaredShapes
	*/
	void							setOriginType(OriginType ot);
	void							setOriginType(std::string& str);
	void							applyImpulse(const b2Vec2& force);

	////////////////////////////////////////////////////////////
	// 1.2 Fixture elements
	////////////////////////////////////////////////////////////
	void							setFixtureData(float friction, float density, float restitution);
	void							setFixtureFriction(float friction);
	void							setFixtureDensity(float density);
	void							setFixtureRestitution(float restitution);
	void							setFixtureSensor(bool sensor);
	

	////////////////////////////////////////////////////////////
	// 1.3 FilteringCollisionData
	////////////////////////////////////////////////////////////
	void							setFixtureFilterData(const b2Filter&);

	void							setFixtureCollisionMask(CollisionGroup);
	void							addFixtureCollisionMask(CollisionGroup);
	void							removeFixtureCollisionMask(CollisionGroup);
	void							setFixtureCategoryMask(CollisionGroup);

	////////////////////////////////////////////////////////////
	// 1.4 Body elements
	////////////////////////////////////////////////////////////
	void							setBodyRotation(bool fixedrotation);
	void							setBodyType(b2BodyType);

	////////////////////////////////////////////////////////////
	// 1.5 General elements
	////////////////////////////////////////////////////////////
	static void						setWorld(b2World* World);

	template<typename T>
	void							setType(){
		m_type = &typeid(T);
	}
	////////////////////////////////////////////////////////////
	// 2 Getters
	////////////////////////////////////////////////////////////

	static const b2World*			getWorld();
	static const float				getScale();
	static const float32			getBodyScale(){ return c_BodyScale; }

	const b2Vec2&					getHitBoxB2() const;
	b2Vec2							getPositionB2() const;
	const b2Vec2&					getSpeed() const;
	float							getAngle() const;
	const b2Filter&					getFilterData() const { return m_body->GetFixtureList()->GetFilterData(); };
	float							getMass() { return m_body->GetMass(); };
	const std::type_info&			getType() { return *m_type; };
	////////////////////////////////////////////////////////////
	// Return datas from the body
	// @return [0] = Density
	// @return [1] = Friction
	// @return [2] = Restitution
	////////////////////////////////////////////////////////////
	float*							getData() const;

	////////////////////////////////////////////////////////////
	//Function executed when the body start a collision.
	//@param : b2Body -> the body with wich the PhysBody collision.
	//@param : b2Manifold -> some data concerning the collision.	
	////////////////////////////////////////////////////////////
	virtual void					startContact(PhysBody*, const b2Manifold&){};


	////////////////////////////////////////////////////////////
	//Function executed when the body end a collision.
	//@param : b2Body -> the body with wich the PhysBody collision.
	//@param : b2Manifold -> some data concerning the collision.
	////////////////////////////////////////////////////////////
	virtual void					endContact(PhysBody*, const b2Manifold&){};
private: // ------- Functions
	b2Vec2							getOriginWorker(const b2Vec2& size);
private: // ------- Members
	//Static constant element:
	//Physical World were the body will be created.
	static b2World*					c_World;

	//The scale change between the input and inner values.
	static const float32			c_BodyScale;

	//Pointer on the body
	b2Body*							m_body;
										
	//Size of the body
	b2Vec2							m_size;
	
	//class type
	const std::type_info*			m_type;

	//origin type
	OriginType						m_originType;
protected: //-------- Functions 
	////////////////////////////////////////////////////////////
    // Theses Functions are usefull for the childs.
    ////////////////////////////////////////////////////////////

	template<typename T>
	static sf::Vector2<T> BtoSF(const b2Vec2& vector){
		return sf::Vector2<T>((T)vector.x, (T)vector.y);
	}
	static b2Vec2 SFtoB(const sf::Vector2f& vector){
		return b2Vec2((float32)vector.x, (float32)vector.y);
	}
	static b2Vec2 SFtoB(const sf::Vector2i& vector){
		return b2Vec2((float32)vector.x, (float32)vector.y);
	}
	static b2Vec2 SFtoB(const sf::Vector2u& vector){
		return b2Vec2((float32)vector.x, (float32)vector.y);
	}
};

#endif