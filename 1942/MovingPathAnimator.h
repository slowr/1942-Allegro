#pragma once

#include "Animator.h"
#include "MovingPathAnimation.h"

class MovingPathAnimator : public Animator {
	Sprite* sprite;
	MovingPathAnimation* anim;
public:
	virtual void MovingPathAnimator::Start(Sprite* s, Animation* a);
	MovingPathAnimator(void);
	~MovingPathAnimator();
	void Progress();
};
