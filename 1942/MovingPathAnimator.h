#pragma once

#include "Animator.h"
#include "MovingPathAnimation.h"

class MovingPathAnimator : public Animator {
	Sprite* sprite;
	MovingPathAnimation* anim;
public:
	void Start(Sprite* s, MovingPathAnimation* a, timestamp_t t);
	MovingPathAnimator(void);
	~MovingPathAnimator();
	void Progress(timestamp_t currTime);
};
