#pragma once

#include "Animation.h"
#include "MovingAnimation.h"
#include <list>

class FrameListAnimation : public MovingAnimation {
	std::list<frame_t> frames;
public:
	const std::list<frame_t>& GetFrames(void) const;
	void SetFrames(const std::list<frame_t>& f);
	Animation* Clone(animid_t newId) const;
	FrameListAnimation(
		const std::list<frame_t>& _frames,
		offset_t dx, offset_t dy, delay_t d, bool c, animid_t id
		);
};