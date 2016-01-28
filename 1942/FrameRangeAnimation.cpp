#include "FrameRangeAnimation.h"

frame_t FrameRangeAnimation::GetStartFrame(void) const { 
	return start; 
}

frame_t FrameRangeAnimation::GetEndFrame(void) const {
	return end; 
}

void FrameRangeAnimation::SetStartFrame(frame_t v) {
	start = v; 
}

void FrameRangeAnimation::SetEndFrame(frame_t v) {
	end = v; 
}

Animation* FrameRangeAnimation::Clone(animid_t newId) const {
	return new FrameRangeAnimation(
		start, end, GetDx(), GetDy(), GetDelay(), GetContinuous(), newId
		);
}

FrameRangeAnimation::FrameRangeAnimation(
	frame_t s, frame_t e,
	offset_t dx, offset_t dy, delay_t d, bool c, animid_t id
	) : start(s), end(e), MovingAnimation(dx, dy, d, c, id) {}