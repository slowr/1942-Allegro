#pragma once

#include "Animator.h"
#include "MovingAnimation.h"

class MovingAnimator : public Animator {
	Sprite* sprite;
	MovingAnimation* anim;
public:
	void Start(Sprite* s, MovingAnimation* a, timestamp_t t);
	MovingAnimator(void);
	~MovingAnimator();
	void Progress(timestamp_t currTime);
};
