////////////////////////////////////////////////////////////
// < Fichier : "Application.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef APPLICATION_H
#define APPLICATION_H
#include "Singleton.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Parameters.h"
#include "constant.h"
#include "Swarm.h"
#include <Box2D/Dynamics/b2World.h>

class Application : public Singleton<Application>
{
	friend class Singleton < Application >;
public:
	void launch();
	void close();

	sf::Vector2u		getWindowSize() const { return m_Window.getSize(); }

	void togglePause()	{ m_launched = !m_launched; }
	void addBubule();
	void removeBubule();
private:
	Application();
	~Application();

	void setBorders();

	void enableNotif();
	void enableWindow();

	sf::RenderWindow	m_Window;
	b2World				m_World;
	Swarm				m_Swarm;
	sf::Clock			m_Clock;
	sf::Time			m_Time;

	bool				m_launched;
};


#endif 