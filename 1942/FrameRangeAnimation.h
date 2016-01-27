#ifndef _FRAMERANGEANIMATION_H_
#define _FRAMERANGEANIMATION_H_

#pragma once

#include "Animation.h"
#include "MovingAnimation.h"

class FrameRangeAnimation : public MovingAnimation {
	frame_t start, end;
public:
	frame_t GetStartFrame(void) const { return start; }
	frame_t GetEndFrame(void) const { return end; }
	void SetStartFrame(frame_t v) { start = v; }
	void SetEndFrame(frame_t v) { end = v; }
	Animation* Clone(animid_t newId) const {
		return new FrameRangeAnimation(
			start, end, GetDx(), GetDy(), GetDelay(), GetContinuous(), newId
			);
	}
	FrameRangeAnimation(
		frame_t s, frame_t e,
		offset_t dx, offset_t dy, delay_t d, bool c, animid_t id
		) : start(s), end(e), MovingAnimation(dx, dy, d, c, id) {}
};

#endif