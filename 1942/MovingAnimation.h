#ifndef _MOVINGANIMATION_H_
#define _MOVINGANIMATION_H_

#pragma once
#include "Animation.h"

class MovingAnimation : public Animation {
	offset_t dx, dy;
	delay_t delay;
	bool continuous;
public:
	offset_t GetDx(void) const { return dx; }
	void SetDx(offset_t v) { dx = v; }
	offset_t GetDy(void) const { return dy; }
	void SetDy(offset_t v) { dy = v; }
	delay_t GetDelay(void) const { return delay; }
	void SetDelay(delay_t v) { delay = v; }
	bool GetContinuous(void) const { return continuous; }
	void SetContinuous(bool v) { continuous = v; }
	Animation* Clone(animid_t newId) const
	{
		return new MovingAnimation(dx, dy, delay, continuous, newId);
	}
	MovingAnimation(
		offset_t _dx, offset_t _dy, delay_t _delay, bool c, animid_t _id
		) : dx(_dx), dy(_dy), delay(_delay), continuous(c), Animation(_id) {}
};

#endif