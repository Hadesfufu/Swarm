#ifndef SOUNDHOLDER_H
#define SOUNDHOLDER_H

#include <string>
#include <map>
#include <memory>
#include "Log.h"
#include "SFML\Audio.hpp"
#include "Singleton.h"

////////////////////////////////////////////////////////////
//
//-------------------BY HadesFufu---------------------
// >French<
//Utilisation : Cette classe sert de stockage aux differents sons du jeu
//afin de ne les charger qu'une seule fois dans la m�moire.
//A chaque recup�ration de sons externe -> pass� par cette classe.
//
//Elements :
// Un son est un fichier court, charger en m�moire qui a pour vocation d'�tre
// utilis� souvent. 
// Exemple : Bruit de pas, saut ...
//
//Fonctionnement : Prend le chemin du son. la stocker si il existe 
//et qu'il n'est pas d�ja dans la m�moire. Retourne un pointeur sur 
//le SoundBuffer.
//
////////////////////////////////////////////////////////////
class SoundHolder : public Singleton<SoundHolder>
{
	friend class Singleton<SoundHolder>;
public:
	////////////////////////////
	// Main Function
	// @effect:		if the texture is arleady loaded, it returns a pointer on it,
	// @effect:		else it load the texture and stock it into : m_textures.
	// @param:	(const std::string)		name of the Texture.
	// @return:		(sf::Texture*)			pointer on the Texture. 
	////////////////////////////
	sf::SoundBuffer* getSound(const std::string&);
private:
	////////////////////////////
	// Constructor
	////////////////////////////
	SoundHolder();
	SoundHolder(const SoundHolder&) = delete;

	////////////////////////////
	// Destructor
	////////////////////////////
	~SoundHolder(){};


	////////////////////////////
	// sf::Texture Storage
	////////////////////////////
	std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_Sounds;

	////////////////////////////
	// Constant path for textures.
	////////////////////////////
	const std::string		m_Chemin_sound;
};

//template Singleton<SoundHolder>;

#endif