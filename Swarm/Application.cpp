#include "Application.h"
#include "Drawer.h"
#include "InputManager.h"
#include "NotificationManager.h"
#include "PhysBody.h"

Application::Application() : m_World(b2Vec2(0.0f, 0.0f)), m_launched(true)
{
	PhysBody::setWorld(&m_World);
	std::srand(0);
	enableWindow();
	enableNotif();
	setBorders();
	m_Swarm.createChild(20);
	m_Swarm.goToCenter();
	//m_Swarm.doACircle();
	//m_Swarm.doALine();
}


Application::~Application()
{
	NotificationManager::I()->RemoveObserver(this);
}

void Application::launch()
{
	sf::Event e;
	while (m_Window.isOpen()){
		m_Time = m_Clock.getElapsedTime();
		m_Clock.restart();

		while (m_Window.pollEvent(e))
		{
			Input::Manager::I()->handelInputEvent(e);
		}
		
		Input::Manager::I()->notifyClickingAction();

		if (m_launched){
			m_Swarm.update(m_Time);
			m_World.Step(m_Time.asSeconds(), 8, 3);
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
		m_Window.create(sf::VideoMode(width, height), "Swarm");
	}
	else if (windowMode == Constant::SELECTOR_WINDOWMODE_FULLSCREEN){
		m_Window.create(sf::VideoMode().getDesktopMode(), "Swarm", sf::Style::Fullscreen);
	}
	else if (windowMode == Constant::SELECTOR_WINDOWMODE_BORDERLESS){
		int width = Parameters::I()->getVec2I("WindowSize")->x;
		int height = Parameters::I()->getVec2I("WindowSize")->y;
		m_Window.create(sf::VideoMode(width, height), "Swarm", sf::Style::None);
	}
	else{
		m_Window.create(sf::VideoMode().getDesktopMode(), "Swarm", sf::Style::None);
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
	NotificationManager::I()->AddObserver("pause", this, &Application::togglePause);
	NotificationManager::I()->AddObserver("add", this, &Application::addBubule);
	NotificationManager::I()->AddObserver("remove", this, &Application::removeBubule);
}

void Application::close()
{
	m_Window.close();
}

void Application::setBorders(){
	b2Body* m_body;
	b2EdgeShape shape;
	b2BodyDef def;
	def.type = b2_staticBody;
	sf::Vector2u size = m_Window.getSize();

	//Top
	def.position = b2Vec2(0, 0);
	m_body = m_World.CreateBody(&def);
	shape.Set(b2Vec2(0, 0), b2Vec2((float32)size.x / PhysBody::getBodyScale(), 0));
	m_body->CreateFixture(&shape, 0.0f);

	//Left
	def.position = b2Vec2(0, 0);
	m_body = m_World.CreateBody(&def);
	shape.Set(b2Vec2(0, 0), b2Vec2(0, (float32)size.y / PhysBody::getBodyScale()));
	m_body->CreateFixture(&shape, 0.0f);

	//Right
	def.position = b2Vec2((float32)size.x / PhysBody::getBodyScale(), 0);
	m_body = m_World.CreateBody(&def);
	shape.Set(b2Vec2(0, 0), b2Vec2(0, (float32)size.y / PhysBody::getBodyScale()));
	m_body->CreateFixture(&shape, 0.0f);

	//Bot
	def.position = b2Vec2(0, (float32)size.y / PhysBody::getBodyScale());
	m_body = m_World.CreateBody(&def);
	shape.Set(b2Vec2(0, 0), b2Vec2((float32)size.x / PhysBody::getBodyScale(), 0));
	m_body->CreateFixture(&shape, 0.0f);
}

void Application::addBubule()
{
	sf::Vector2f mousePos = (sf::Vector2f)Input::Manager::I()->getMousePos();
	m_Swarm.createChild(mousePos);

}

void Application::removeBubule()
{
	sf::Vector2f mousePos = (sf::Vector2f)Input::Manager::I()->getMousePos();
	m_Swarm.deleteChildAt(mousePos);
}