////////////////////////////////////////////////////////////
// < Fichier : "Swarm_child.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef SWARM_CHILD_H
#define SWARM_CHILD_H
#include <SFML/Graphics.hpp>
#include "Behavior.h"
#include "Swarm.h"

class Swarm_child : public sf::Drawable, public sf::Transformable
{
public:
	Swarm_child(Swarm* swarm);
	~Swarm_child();

	virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2f	getPosition(){ return m_shape->getPosition(); };
	sf::Color		getColor(){ return m_shape->getFillColor(); }
	const Swarm*	getSwarm(){ return m_swarm_; }
	int				getId(){ return m_id; }

	void			setBehavior(Behavior*);
	
	void			stop();

	void			moveTo(const sf::Vector2f& pos);

private:

	void update();

	sf::Shape*	m_shape;
	sf::Thread	m_thread_;

	sf::Clock	m_clock;
	
	Behavior*	m_Behavior;
	Swarm*		m_swarm_;
	
	float		m_speed;
	int			m_id;

	static int	m_currentId;
};


#endif 