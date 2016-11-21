////////////////////////////////////////////////////////////
// < Fichier : "DrawingAnimationFade.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef DRAWINGANIMATIONFADE_H
#define DRAWINGANIMATIONFADE_H


#include "DrawingAnimation.h"

enum FadeType
{ 
	FadeOut, 
	FadeIn 
};

class DrawingAnimationFade : public DrawingAnimation
{
public:
	DrawingAnimationFade(sf::Time duration, sf::Sprite* elem, FadeType ft);
	~DrawingAnimationFade();

	void effect(float percent);

private: 
	FadeType m_FadeType;
};

#endif 