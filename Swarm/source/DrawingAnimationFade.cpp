
#include "DrawingAnimationFade.h"
#include "Log.h"

DrawingAnimationFade::DrawingAnimationFade(sf::Time duration, sf::Sprite* elem, FadeType ft): DrawingAnimation(duration, elem), m_FadeType(ft)
{

}
DrawingAnimationFade::~DrawingAnimationFade()
{

}

void DrawingAnimationFade::effect(float percent){
	sf::Sprite* sprite = static_cast<sf::Sprite*>(m_element);
	if (sprite){
		if (m_FadeType == FadeIn)
			sprite->setColor(sf::Color(255, 255, 255, (unsigned int)(percent * 255)));
		else
			sprite->setColor(sf::Color(255, 255, 255, (unsigned int)((1.0f - percent) * 255)));
	}
	else
		Log::error() << "bug cast";
}