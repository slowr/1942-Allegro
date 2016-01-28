#include "MovingAnimation.h"

offset_t MovingAnimation::GetDx(void) const { 
	return dx; 
}

void MovingAnimation::SetDx(offset_t v) { 
	dx = v; 
}

offset_t MovingAnimation::GetDy(void) const { 
	return dy; 
}

void MovingAnimation::SetDy(offset_t v) { 
	dy = v; 
}

delay_t MovingAnimation::GetDelay(void) const { 
	return delay; 
}

void MovingAnimation::SetDelay(delay_t v) { 
	delay = v; 
}

bool MovingAnimation::GetContinuous(void) const { 
	return continuous; 
}

void MovingAnimation::SetContinuous(bool v) { 
	continuous = v; 
}

Animation* MovingAnimation::Clone(animid_t newId) const{
	return new MovingAnimation(dx, dy, delay, continuous, newId);
}

MovingAnimation::MovingAnimation(
	offset_t _dx, offset_t _dy, delay_t _delay, bool c, animid_t _id
	) : dx(_dx), dy(_dy), delay(_delay), continuous(c), Animation(_id) {}