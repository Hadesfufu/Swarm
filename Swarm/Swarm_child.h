////////////////////////////////////////////////////////////
// < Fichier : "Swarm_child.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef SWARM_CHILD_H
#define SWARM_CHILD_H
#include <SFML/Graphics/Drawable.hpp>

class Swarm_child : public sf::Drawable
{
public:
	Swarm_child();
	~Swarm_child();

	virtual void draw(RenderTarget& target, sf::RenderStates states) const;
};


#endif 