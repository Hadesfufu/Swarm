
#include "SFML\System.hpp"
#include <future>
#include <functional>
#include "InputManager.h"
#include "NotificationManager.h"
#include "Parameters.h"
#include <windows.h>

#define INTERVAL_CLICKING_SECOND			.33f

#define KEY_TYPE_KEYBOARD					0
#define KEY_TYPE_MOUSE						1
#define KEY_TYPE_JOYSTICK					2

#define INPUTMAP_PARAMETER_PATH_STR			"InputMap"
#define INPUTMAP_DEFAULT_JOYSTICK_STR		"inputDefaultJoystick"
#define INPUTMAP_DEFAULT_KEY_STR			"inputDefaultKeyboardKey"

wchar_t *convertCharArrayToLPCWSTR2(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}


namespace Input{
	Manager::Manager() :
		m_usingJoystick(false)
	{
		if (loadInput()){
			Log::info("InputManager") << "-------	InputMap loaded		------\n";
		}
		else{
			Log::error("InputManager") << "-------	InputMap loading error		------\n";
			GfxDbgAssert(false, "error while loading InputMap");
		}
	}
	Manager::~Manager()
	{
		const std::string *inputMapPathPtr = Parameters::I()->getString(INPUTMAP_PARAMETER_PATH_STR);
		if (!inputMapPathPtr){
			Log::error("InputManager") << "can not find inputMap path in Parameters file, save unabled\n";
		}

		m_inputFile.save_file(inputMapPathPtr->c_str());
	}

	bool Manager::handelInputEvent(sf::Event windowEvent){
		switch (windowEvent.type){
		case sf::Event::Closed:
			NotificationManager::I()->PostNotification(NOTIFICATION_NAME_CLOSING_EVENT);
			break;
		case sf::Event::Resized:{
			break;
		}
		case sf::Event::LostFocus:{
			break;
		}
		case sf::Event::GainedFocus:{
			break;
		}
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
			return handleKeyboardEvent(windowEvent);
			break;
		case sf::Event::MouseWheelMoved:
		case sf::Event::MouseWheelScrolled:
			break;
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		case sf::Event::MouseEntered:
		case sf::Event::MouseLeft:
		case sf::Event::MouseMoved:
			return handleMouseEvent(windowEvent);
			break;
		case sf::Event::JoystickButtonPressed:
		case sf::Event::JoystickButtonReleased:
		case sf::Event::JoystickConnected:
		case sf::Event::JoystickDisconnected:
		case sf::Event::JoystickMoved:
			break;
		case sf::Event::TextEntered:
			break;
		default:
			Log::warning("InputManager") << "error invalide event to handle";
			return false;
			break;
		}

		return true;
	}

	bool Manager::notifyClickingAction(){
		for (auto it = m_pressedCtimeForActions.begin(); it != m_pressedCtimeForActions.end(); it++){
			TimerForKey timer = it->second;
			sf::Time clickingTime = timer.m_clock.getElapsedTime();
			if (clickingTime.asSeconds() >= INTERVAL_CLICKING_SECOND){
				timer.m_clock.restart();
				timer.m_duration += clickingTime;
				NotificationManager::Dictionary userInfo;
				userInfo.m_dict["timeClicking"] = &timer.m_duration;
				NotificationManager::I()->PostNotification(it->first, userInfo);
				it->second = timer;
			}
		}

		return true;
	}

	const std::list<Key> Manager::getKeyForAction(char *actionEventStr) const{
		return m_keyboardActions.keyForSecondKey(actionEventStr);
	}

	bool Manager::resetKeyForEvent(char *actionEventStr, Key currentKey){
		bool linked = m_keyboardActions.unlink(currentKey, actionEventStr);
		if (!linked){
			Log::error("InputManager") << "can not reset key, wrong old kay to unbind set";
			return false;
		}

		return loadDefaultKeyForAction(actionEventStr, m_usingJoystick);
	}

	bool Manager::setKeyForEvent(char *actionEventStr){

		return true;
	}

	bool Manager::loadDefaultKeyForAction(std::string actionStr, bool joystick){
		auto elem = m_defaultKey.find(actionStr);
		if (elem == m_defaultKey.end()){
			Log::error("inputManager") << "error : no default key for action " << actionStr;
			return false;
		}

		auto inputs = elem->second;
		std::map<std::string, Key>::iterator input;
		if (joystick){
			input = inputs.find(INPUTMAP_DEFAULT_JOYSTICK_STR);
		}
		else{
			char keyboardLayoutName[KL_NAMELENGTH];
			keyboardLayoutName[0] = 0;
			GetKeyboardLayoutName(convertCharArrayToLPCWSTR2(keyboardLayoutName));

			input = inputs.find(keyboardLayoutName);
			if (input == inputs.end())
				input = inputs.find(INPUTMAP_DEFAULT_KEY_STR);
		}

		GfxDbgAssert(input != inputs.end());

		return m_keyboardActions.link(input->second, actionStr);
	}

	bool Manager::loadInput(){
		const std::string *inputMapPathPtr = Parameters::I()->getString(INPUTMAP_PARAMETER_PATH_STR);
		if (!inputMapPathPtr || !m_inputFile.load_file(inputMapPathPtr->c_str())){
			Log::error("InputManager") << "can not find inputMap path in Parameters file";
			return false;
		}
 		pugi::xml_node inputs = m_inputFile.child("map");
		pugi::xml_node input = inputs.first_child();
		while (input){
			loadAction(input);
			input = input.next_sibling();
		}

		return true;
	}
	void Manager::loadAction(pugi::xml_node actionXml){
		std::string action = actionXml.attribute("action").as_string();
		auto defaultActions = m_defaultKey.find(action);
		if (defaultActions == m_defaultKey.end()){
			defaultActions = m_defaultKey.emplace(action, std::map<std::string, Key>()).first;
		}

		pugi::xml_node localisation = actionXml.child("keyboard");
		while (localisation){
			std::string layout = localisation.attribute("type").as_string();
			std::string code = localisation.attribute("code").as_string();

			Key linkedKey = keyForTypeAndId(KEY_TYPE_KEYBOARD, code);
			auto paired = std::pair<std::string, Key>(layout, linkedKey);
			defaultActions->second.insert(paired);

			localisation = localisation.next_sibling("keyboard");
		}

		pugi::xml_node defaultInput = actionXml.child("default");
		unsigned int type = defaultInput.attribute("type").as_uint();
		std::string code = defaultInput.attribute("code").as_string();
		auto pairedKey = std::pair<std::string, Key>(INPUTMAP_DEFAULT_KEY_STR, keyForTypeAndId(type, code));
		defaultActions->second.insert(pairedKey);

		pugi::xml_node defaultJoystick = actionXml.child("defaultJoystick");
		std::string codeJoystick = defaultJoystick.attribute("code").as_string();
		auto pairedKeyJoystick = std::pair<std::string, Key>(INPUTMAP_DEFAULT_JOYSTICK_STR, keyForTypeAndId(KEY_TYPE_JOYSTICK, codeJoystick));
		defaultActions->second.insert(pairedKeyJoystick);

		pugi::xml_attribute userType = actionXml.attribute("userType");
		pugi::xml_attribute userCode = actionXml.attribute("userCode");
		if (userType && userCode){
			Key userKey = keyForTypeAndId(userType.as_uint(), userCode.as_string());
			m_keyboardActions.link(userKey, action);
		}
		else
			loadDefaultKeyForAction(action, false);

		pugi::xml_attribute userJoystick = actionXml.attribute("userJoystick");
		if (userJoystick){
			Key userKey = keyForTypeAndId(KEY_TYPE_JOYSTICK, userCode.as_string());
			m_keyboardActions.link(userKey, action);
		}
		else 
			loadDefaultKeyForAction(action, true);
	}
	Key Manager::keyForTypeAndId(unsigned int type, std::string id){
		switch (type){
		case KEY_TYPE_KEYBOARD:{
			size_t delimitierPos = 0;

			delimitierPos = id.find("_");
			if (delimitierPos == std::string::npos){
				Log::error("inputManager") << "error loading key : " << type << " " << id;
				return Key::ErrorKey();
			}
			std::string keyCode = id.substr(0, delimitierPos);
			id = id.substr(delimitierPos + 1);

			delimitierPos = id.find("_");
			if (delimitierPos == std::string::npos){
				Log::error("inputManager") << "error loading key : " << type << " " << id;
				return Key::ErrorKey();
			}
			std::string keyStatus = id.substr(0, delimitierPos);
			id = id.substr(delimitierPos + 1);

			delimitierPos = id.find("_");
			if (delimitierPos == std::string::npos){
				Log::error("inputManager") << "error loading key : " << type << " " << id;
				return Key::ErrorKey();
			}
			std::string keyAltStatus = id.substr(0, delimitierPos);
			id = id.substr(delimitierPos + 1);

			delimitierPos = id.find("_");
			if (delimitierPos == std::string::npos){
				Log::error("inputManager") << "error loading key : " << type << " " << id;
				return Key::ErrorKey();
			}
			std::string keyShiftStatus = id.substr(0, delimitierPos);
			id = id.substr(delimitierPos + 1);

			delimitierPos = id.find("_");
			if (delimitierPos == std::string::npos){
				Log::error("inputManager") << "error loading key : " << type << " " << id;
				return Key::ErrorKey();
			}
			std::string keyCtrlStatus = id.substr(0, delimitierPos);
			id = id.substr(delimitierPos + 1);

			std::string keyWindowStatus = id;
			
			return Key::KeyForKeyboard((sf::Keyboard::Key)std::stoi(keyCode),
				(EKeyStatus)std::stoi(keyStatus),
				std::stoi(keyAltStatus) == 1,
				std::stoi(keyShiftStatus) == 1,
				std::stoi(keyCtrlStatus) == 1,
				std::stoi(keyWindowStatus) == 1);
		}
		case KEY_TYPE_MOUSE:{
			size_t delimitierPos = 0;

			delimitierPos = id.find("_");
			if (delimitierPos == std::string::npos){
				Log::error("inputManager") << "error loading key : " << type << " " << id;
				return Key::ErrorKey();
			}
			std::string keyCode = id.substr(0, delimitierPos);
			id = id.substr(delimitierPos + 1);

			std::string keyStatus = id;

			return Key::KeyForMouse((EKeyStatus)std::stoi(keyStatus),
				(sf::Mouse::Button)std::stoi(keyCode));
		}
		default:
			return Key::ErrorKey();
		}
	}

	bool Manager::handleKeyboardEvent(sf::Event &keyboardEvent){
		switch (keyboardEvent.type){
		case sf::Event::KeyPressed:{
			Key key = Key::KeyForKeyboard(keyboardEvent.key.code, Input::EKeyStatus_Pressed, keyboardEvent.key.alt, keyboardEvent.key.shift, keyboardEvent.key.control, keyboardEvent.key.system);
			std::list<std::string> actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end()){
					NotificationManager::I()->PostNotification(*it);
				}
			}

			key = Key::KeyForKeyboard(keyboardEvent.key.code, Input::EKeyStatus_Pressing, keyboardEvent.key.alt, keyboardEvent.key.shift, keyboardEvent.key.control, keyboardEvent.key.system);
			actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end()){
					TimerForKey timer;
					timer.m_clock = sf::Clock();
					timer.m_duration = sf::Time::Zero;
					auto paired = std::pair<std::string, TimerForKey>(*it, timer);
					m_pressedCtimeForActions.insert(paired);
				}
			}

			return true;
		}
		case sf::Event::KeyReleased:{
			Key key = Key::KeyForKeyboard(keyboardEvent.key.code, Input::EKeyStatus_Pressing, keyboardEvent.key.alt, keyboardEvent.key.shift, keyboardEvent.key.control, keyboardEvent.key.system);
			std::list<std::string> actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				auto it2 = m_pressedCtimeForActions.find(*it);
				if (it2 != m_pressedCtimeForActions.end())
					m_pressedCtimeForActions.erase(it2);
			}

			key = Key::KeyForKeyboard(keyboardEvent.key.code, Input::EKeyStatus_Release, keyboardEvent.key.alt, keyboardEvent.key.shift, keyboardEvent.key.control, keyboardEvent.key.system);
			actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end())
					NotificationManager::I()->PostNotification(*it);
			}

			return true;
		}
		default:
			return false;
		}

		return true;
	}
	bool Manager::handleMouseEvent(sf::Event &mouseEvent){
		switch (mouseEvent.type){
		case sf::Event::MouseButtonPressed:{
			Key key = Key::KeyForMouse(Input::EKeyStatus_Pressed, mouseEvent.mouseButton.button);
			std::list<std::string> actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end())
					NotificationManager::I()->PostNotification(actionName);
			}

			key = Key::KeyForMouse(Input::EKeyStatus_Pressing, mouseEvent.mouseButton.button);
			actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end()){
					TimerForKey timer;
					timer.m_clock = sf::Clock();
					timer.m_duration = sf::Time::Zero;
					auto paired = std::pair<std::string, TimerForKey>(*it, timer);
					m_pressedCtimeForActions.insert(paired);
				}
			}

			return true;
		}
		case sf::Event::MouseButtonReleased:{
			Key key = Key::KeyForMouse(Input::EKeyStatus_Pressing, mouseEvent.mouseButton.button);
			std::list<std::string> actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				auto it2 = m_pressedCtimeForActions.find(*it);
				if (it2 != m_pressedCtimeForActions.end())
					m_pressedCtimeForActions.erase(it2);
			}

			key = Key::KeyForMouse(Input::EKeyStatus_Release, mouseEvent.mouseButton.button);
			actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end())
					NotificationManager::I()->PostNotification(actionName);
			}

			return true;
		}
		case sf::Event::MouseEntered:
			break;
		case sf::Event::MouseLeft:
			break;
		case sf::Event::MouseMoved:{
			m_mousePos.x = mouseEvent.mouseMove.x;
			m_mousePos.y = mouseEvent.mouseMove.y;
			NotificationManager::Dictionary userInfo;
			userInfo.m_dict[NOTIFICATION_MOUSE_POSITION] = &m_mousePos;
			NotificationManager::I()->PostNotification(NOTIFICATION_NAME_MOUSE_MOVE_EVENT, userInfo);
			return true;
		}
		default:
			return false;
		}

		return true;
	}
}