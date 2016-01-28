#pragma once

#include "Animation.h"

class FlashAnimation : public Animation {
	frame_t repetitions;
	delay_t hideDelay;
	delay_t showDelay;
public:
	void SetRepetitions(frame_t n);
	frame_t GetRepetitions(void) const;
	void SetHideDeay(delay_t d);
	delay_t GetHideDeay(void) const;
	void SetShowDeay(delay_t d);
	delay_t GetShowDeay(void) const;
	Animation* Clone(animid_t newId) const;
	FlashAnimation(frame_t n, delay_t show, delay_t hide, animid_t id);
};
