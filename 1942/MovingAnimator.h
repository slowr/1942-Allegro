#pragma once

#include "Animator.h"
#include "MovingAnimation.h"

class MovingAnimator : public Animator {
	Sprite* sprite;
	MovingAnimation* anim;
public:
	virtual void Start(Sprite* s, Animation* a);
	MovingAnimator(void);
	~MovingAnimator();
	void Progress();
};
