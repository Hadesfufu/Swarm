////////////////////////////////////////////////////////////
// < Fichier : "E_Manager.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef E_MANAGER_H
#define E_MANAGER_H

#include "Singleton.h"
#include "E_Command.h"
#include <map>

template class std::multimap<std::size_t, std::pair<std::string, std::exception*>>;

class E_Manager : public Singleton<E_Manager>
{
	friend class Singleton < E_Manager >;

public:
	template<class Issuer, class Receiver>
	void launchCommand(Issuer* issuer, std::string func){
		auto it1 = m_Commands.equal_range(typeid(Receiver).hash_code());
		for (auto it2 = it1.first; it2 != it1.second; ++it2)
		{
			if (it2->second.first == func){
				std::exception* ex = it2->second.second;
				E_Command<Issuer, Receiver> thrower(*static_cast<E_Command<Issuer, Receiver>*>(ex));
				thrower.setIssuer(issuer);
				throw thrower;
				break;
			}
		}
		Log::debug() << "Command not found !";
	}

	template<class Issuer, class Receiver>
	void add(void(Receiver::* f)(Issuer*), std::string name){
		std::size_t info = typeid(Receiver).hash_code();
		m_Commands.emplace(info, std::pair<std::string, std::exception*>(name, new E_Command<Issuer, Receiver>(f)));
	}
	template<class Issuer, class Receiver>
	void add(void(Receiver::* f)(void), std::string name){
		std::size_t info = typeid(Receiver).hash_code();
		m_Commands.emplace(info, std::pair<std::string, std::exception*>(name, new E_Command<Issuer, Receiver>(f)));
	}
private:
	E_Manager();
	~E_Manager();

private:
	std::multimap<std::size_t, std::pair<std::string, std::exception*>> m_Commands;
};

#endif 