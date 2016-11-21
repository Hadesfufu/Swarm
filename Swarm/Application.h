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

class Application : public Singleton<Application>
{
	friend class Singleton < Application >;
public:
	void launch();
	void close();
private:
	Application();
	~Application();

	void enableNotif();
	void enableWindow();

	sf::RenderWindow m_Window;
	Swarm			 m_Swarm;
};


#endif 