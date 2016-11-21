#include "Application.h"
#include "Drawer.h"
#include "InputManager.h"
#include "E_Manager.h"
#include "NotificationManager.h"

Application::Application()
{
	enableWindow();
	enableNotif();
	m_Swarm.createChild(12);
}


Application::~Application()
{
	NotificationManager::I()->RemoveObserver(this);
}

void Application::launch()
{
	sf::Event e;
	while (m_Window.isOpen()){
		while (m_Window.pollEvent(e))
		{
			try {
				Input::Manager::I()->handelInputEvent(e);
			}
			catch (E_Base<Application>& e) {
				if (e.getFunction() != nullptr)
					e.getFunction()(this);
			}
		}

		try {
			Input::Manager::I()->notifyClickingAction();

		}
		catch (E_Base<Application>& e) {
			if (e.getFunction() != nullptr)
				e.getFunction()(this);
		}

		Drawer::I()->draw();
	}
}

void Application::enableWindow()
{
	int windowMode = *Parameters::I()->getInt("FullScreen");
	sf::ContextSettings context;
	context.antialiasingLevel = *Parameters::I()->getInt("AntiAliasing");
	if (windowMode == Constant::SELECTOR_WINDOWMODE_WINDOWED){
		int width = Parameters::I()->getVec2I("WindowSize")->x;
		int height = Parameters::I()->getVec2I("WindowSize")->y;
		m_Window.create(sf::VideoMode(width, height), "Zeraventure");
	}
	else if (windowMode == Constant::SELECTOR_WINDOWMODE_FULLSCREEN){
		m_Window.create(sf::VideoMode().getDesktopMode(), "Zeraventure", sf::Style::Fullscreen);
	}
	else if (windowMode == Constant::SELECTOR_WINDOWMODE_BORDERLESS){
		int width = Parameters::I()->getVec2I("WindowSize")->x;
		int height = Parameters::I()->getVec2I("WindowSize")->y;
		m_Window.create(sf::VideoMode(width, height), "Zeraventure", sf::Style::None);
	}
	else{
		m_Window.create(sf::VideoMode().getDesktopMode(), "Zeraventure", sf::Style::None);
	}
	m_Window.setFramerateLimit(*Parameters::I()->getInt("FrameRate"));
	m_Window.setVerticalSyncEnabled(*Parameters::I()->getBool("VerticalSync"));
	m_Window.setKeyRepeatEnabled(false);
	m_Window.requestFocus();

	Drawer::I()->setWindow(&m_Window);

}

void Application::enableNotif()
{
	NotificationManager::I()->AddObserver(NOTIFICATION_NAME_CLOSING_EVENT, this, &Application::close);
	NotificationManager::I()->AddObserver("close", this, &Application::close);
}

void Application::close()
{
	m_Window.close();
}
