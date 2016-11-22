////////////////////////////////////////////////////////////
// < Fichier : "Behavior.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include <SFML/System/Time.hpp>

class Behavior
{
public:
	Behavior();
	~Behavior();
	
	virtual void update(const sf::Time&){}
private:

};


#endif 