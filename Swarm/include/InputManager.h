#ifndef InputManager_H
#define InputManager_H

#include <SFML/Graphics.hpp>
#include <map>
#include <list>
#include "Singleton.h"
#include "InputKey.h"
#include "reversableHashTable.h"
#include "pugixml.hpp"

#define WINDOW_MODE_WINDOWED				0
#define WINDOW_MODE_FULLSCREEN				1
#define WINDOW_MODE_BORDERLESS				2
#define WINDOW_MODE_FULLSCREEN_BORDERLESS	3

#define NOTIFICATION_NAME_CLOSING_EVENT		"notificationNameClosingEvent"
#define NOTIFICATION_NAME_RESIZE_EVENT		"notificationNameResizeEvent"
#define NOTIFICATION_NAME_FOCUS_EVENT		"notificationNameFocusEvent"

#define NOTIFICATION_NAME_MOUSE_MOVE_EVENT	"notificationNameMouseMoveEvent"
#define NOTIFICATION_MOUSE_POSITION			"notificationMousePosition"

namespace Input{
	class Manager : public Singleton<Manager>
	{
		friend class Singleton<Manager>;
	public:
		typedef struct PressedData{
			bool		m_isPressed;
			sf::Time	m_timePressed;
		}PressedData;

	public:
		bool							handelInputEvent(sf::Event windowEvent);

		bool							notifyClickingAction();

		const std::list<Key>			getKeyForAction(char *actionName) const;

		PressedData						isActionPressed(char *actionName);

//	use it to reset action according to current keyboard layout
		bool							resetKeyForEvent(char *action, Key currentKey);
//	use it to set action with next keyEvent detected
		bool							setKeyForEvent(char *);

		inline sf::Vector2i				getMousePos()const { return m_mousePos; }

	private:
		typedef struct {
			sf::Clock	m_clock;
			sf::Time	m_duration;
		}TimerForKey;

	private:
		Manager();
		Manager(const Manager&) = delete;
		~Manager();

		void							callClickingActions();

		bool							loadDefaultKeyForAction(std::string, bool);

		bool							loadInput();
//	healpers
		void							loadAction(pugi::xml_node);
		Key								keyForTypeAndId(unsigned int type, std::string id);

		bool							handleKeyboardEvent(sf::Event &);
		bool							handleMouseEvent(sf::Event &);

	private:
		bool												m_usingJoystick;
		pugi::xml_document									m_inputFile;

		ReversableHashTable<Key, std::string>				m_keyboardActions;
		std::map<std::string, std::map<std::string, Key>>	m_defaultKey;
		std::map<std::string, TimerForKey>					m_pressedCtimeForActions;

		sf::Vector2i										m_mousePos;
	}; 
}

#endif