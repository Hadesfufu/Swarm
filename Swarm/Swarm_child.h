////////////////////////////////////////////////////////////
// < Fichier : "Swarm_child.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef SWARM_CHILD_H
#define SWARM_CHILD_H
#include <SFML/Graphics.hpp>
#include "Behavior.h"
#include "Swarm.h"
#include <random>
#include <time.h>
#include <thread>
#include "PhysBody.h"

#if defined (_MSC_VER)  // Visual studio
#define thread_local __declspec( thread )
#elif defined (__GCC__) // GCC
#define thread_local __thread
#endif


class Swarm_child : public sf::Drawable, public PhysBody
{
public:
	Swarm_child(Swarm* swarm);
	~Swarm_child();

	virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2f	getPosition() const { return m_shape->getPosition(); }
	sf::Color		getColor() const { return m_shape->getFillColor(); }
	const Swarm*	getSwarm() const { return m_swarm_; }
	int				getId() const { return m_id; }

	void			setBehavior(Behavior*);
	void			setPosition(const sf::Vector2f&);
	
	void			moveTo(const sf::Vector2f& pos);

	int				intRand(const int & min, const int & max);

	float			getDistanceFromOtherChild(const Swarm_child* child);
	float			getDistanceFromPoint(const sf::Vector2f& vec);
	float			getNewDistanceFromOtherChild(const sf::Vector2f& newpos, const Swarm_child* child);

	void			update(sf::Time& dt);
private:


	float			getSign(float x){ if (x > 0) return 1.f; else if (x < 0) return -1.f; return 0; }

	sf::Shape*		m_shape;
	
	Behavior*		m_Behavior;
	Swarm*			m_swarm_;
	
	float			m_speed;
	int				m_id;

	static int		m_currentId;
};


#endif 