#ifndef _FRAMERANGEANIMATOR_H_
#define _FRAMERANGEANIMATOR_H_


#pragma once

#include "Animator.h"
#include "FrameRangeAnimation.h"
#include <iostream>
class FrameRangeAnimator : public Animator {
	Sprite* sprite;
	FrameRangeAnimation* anim;
	frame_t currFrame;
public:
	void Start(Sprite* s, FrameRangeAnimation* a, timestamp_t t) {
		sprite = s;
		anim = a;
		lastTime = t;
		state = ANIMATOR_RUNNING;
		sprite->SetFrame(currFrame = anim->GetStartFrame());
	}

	FrameRangeAnimator(void) :
		sprite((Sprite*)0),
		anim((FrameRangeAnimation*)0), currFrame(0xFF) {}

	~FrameRangeAnimator() {}

	void FrameRangeAnimator::Progress(timestamp_t currTime) {
		while (currTime > lastTime && currTime - lastTime >= anim->GetDelay()) {
			if (currFrame == anim->GetEndFrame())
				currFrame = anim->GetStartFrame();
			else
				++currFrame;

			sprite->Move(anim->GetDx(), anim->GetDy());
			sprite->SetFrame(currFrame);
			lastTime += anim->GetDelay();

			if (currFrame == anim->GetEndFrame() && !anim->GetContinuous()) {
				state = ANIMATOR_FINISHED;
				NotifyStopped();
				return;
			}
		}
	}
};

#endif