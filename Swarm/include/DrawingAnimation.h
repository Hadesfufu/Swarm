////////////////////////////////////////////////////////////
// < Fichier : "DrawingAnimation.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef DRAWINGANIMATION_H
#define DRAWINGANIMATION_H


#include <memory>
#include <SFML/graphics.hpp>

enum TimingEasing
{ 
	linear, 
	easeInQuad, 
	easeOutQuad, 
	easeInOutQuad, 
	easeInCubic, 
	easeOutCubic, 
	easeInOutCubic, 
	easeInQuart, 
	easeOutQuart, 
	easeInOutQuart, 
	easeInQuint, 
	easeOutQuint, 
	easeInOutQuint, 
	easeInExpo, 
	easeOutExpo, 
	easeInOutExpo, 
	easeInCirc, 
	easeOutCirc, 
	easeInOutCirc, 
	easeInBack, 
	easeOutBack, 
	easeInOutBack 
};

class DrawingAnimation
{
public:
	DrawingAnimation(sf::Time duration, sf::Transformable*);
	DrawingAnimation(const DrawingAnimation&) = delete;
	~DrawingAnimation();

	void							update();
	bool							isAlive(){ return m_isAlive; }
	DrawingAnimation*				setDelay(sf::Time delay){ m_delay = delay; return this; };
	DrawingAnimation*				setEasing(TimingEasing easing){ m_easing = easing; return this; };

protected:
	sf::Transformable*				m_element = nullptr;
	virtual void					effect(float percent) = 0;

private:
	DrawingAnimation(sf::Time duration);
	double				calculatePercent();

	TimingEasing		m_easing;
	bool				m_isAlive;
	sf::Time			m_duration;
	sf::Time			m_delay;
	sf::Clock			m_clock;
};

#endif 