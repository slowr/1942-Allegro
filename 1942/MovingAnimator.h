#ifndef _MOVINGANIMATOR_H_
#define _MOVINGANIMATOR_H_

#pragma once

#include "Animator.h"
#include "MovingAnimation.h"

class MovingAnimator : public Animator {
	Sprite* sprite;
	MovingAnimation* anim;
public:
	void Start(Sprite* s, MovingAnimation* a, timestamp_t t) {
		sprite = s;
		anim = a;
		lastTime = t;
		state = ANIMATOR_RUNNING;
	}

	MovingAnimator(void) :
		sprite((Sprite*)0), anim((MovingAnimation*)0) {}
	~MovingAnimator() {}

	void Progress(timestamp_t currTime) {
		while (currTime > lastTime && currTime - lastTime >= anim->GetDelay()) {
			sprite->Move(anim->GetDx(), anim->GetDy());
			if (!anim->GetContinuous()) {
				state = ANIMATOR_FINISHED;
				NotifyStopped();
			}
			else {
				lastTime += anim->GetDelay();
			}
		}
	}
};

#endif