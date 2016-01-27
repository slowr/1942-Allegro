#ifndef _SCROLLANIMATION_H_
#define _SCROLLANIMATION_H_

#pragma once

#include "Animation.h"
#include <list>

struct ScrollEntry {
	// TODO: HorizScroll, VertScroll types? 
	float h;
	float v;
	delay_t delay;
};

class ScrollAnimation : public Animation {
	std::list<ScrollEntry> scroll;
public:
	const std::list<ScrollEntry>& GetScroll(void) const {
		return scroll;
	}

	void SetScroll(const std::list<ScrollEntry>& p){
		scroll.clear(); 
		scroll = p;
	}

	Animation* Clone(animid_t newId) const {
		return new ScrollAnimation(scroll, newId);
	}

	ScrollAnimation(const std::list<ScrollEntry>& _scroll, animid_t id) :
		scroll(_scroll), Animation(id) {}
};

#endif