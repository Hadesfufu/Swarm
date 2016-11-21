
#include "E_Manager.h"
//#include "MenuElement.h"
//#include "MenuActionElement.h"
//#include "MenuManager.h"
//#include "Application.h"
//#include "Game.h"

E_Manager::E_Manager()
{
/*	add<Menu::ActionElement, Menu::Manager>(&Menu::Manager::Play, "play");
	add<Menu::ActionElement, Menu::Manager>(&Menu::Manager::ResumeGame, "resumeGame");
	add<Menu::ActionElement, Menu::Manager>(&Menu::Manager::Exit, "exit");
	add<Menu::ActionElement, Menu::Manager>(&Menu::Manager::SelectBack, "selectBack");
	add<Menu::ActionElement, Menu::Manager>(&Menu::Manager::saveOptions, "saveOptions");

	add<Game, Application>(&Application::pauseGame, "pauseGame");
	add<Game, Application>(&Application::resumeGame, "resumeGame");

	add<Menu::Element, Application>(&Application::launchGame, "play");
	add<Menu::Element, Application>(&Application::exit, "exit");*/
}
E_Manager::~E_Manager()
{
	for (auto it = m_Commands.begin(); it != m_Commands.end(); ++it){
		delete it->second.second;
	}
	m_Commands.clear();
}