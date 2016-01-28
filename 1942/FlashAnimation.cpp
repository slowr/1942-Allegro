#include "FlashAnimation.h"

void FlashAnimation::SetRepetitions(frame_t n) { 
	repetitions = n; 
}

frame_t FlashAnimation::GetRepetitions(void) const { 
	return repetitions; 
}

void FlashAnimation::SetHideDeay(delay_t d) { 
	hideDelay = d; 
}

delay_t FlashAnimation::GetHideDeay(void) const { 
	return hideDelay; 
}

void FlashAnimation::SetShowDeay(delay_t d) { 
	showDelay = d; 
}

delay_t FlashAnimation::GetShowDeay(void) const { 
	return showDelay; 
}

Animation* FlashAnimation::Clone(animid_t newId) const{
	return new FlashAnimation(repetitions, hideDelay, showDelay, newId);
}

FlashAnimation::FlashAnimation(frame_t n, delay_t show, delay_t hide, animid_t id) :
repetitions(n), hideDelay(hide), showDelay(show), Animation(id) {}
