#include "ScrollAnimation.h"

const std::list<ScrollEntry>& ScrollAnimation::GetScroll(void) const {
	return scroll;
}

void ScrollAnimation::SetScroll(const std::list<ScrollEntry>& p){
	scroll.clear();
	scroll = p;
}

Animation* ScrollAnimation::Clone(animid_t newId) const {
	return new ScrollAnimation(scroll, newId);
}

ScrollAnimation::ScrollAnimation(const std::list<ScrollEntry>& _scroll, animid_t id) :
scroll(_scroll), Animation(id) {}
