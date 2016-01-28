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
	const std::list<ScrollEntry>& GetScroll(void) const;
	void SetScroll(const std::list<ScrollEntry>& p);
	Animation* Clone(animid_t newId) const;
	ScrollAnimation(const std::list<ScrollEntry>& _scroll, animid_t id);
};