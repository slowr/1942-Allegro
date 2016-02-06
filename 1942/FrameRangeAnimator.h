#pragma once

#include "Animator.h"
#include "FrameRangeAnimation.h"
#include <iostream>
class FrameRangeAnimator : public Animator {
	Sprite* sprite;
	FrameRangeAnimation* anim;
	frame_t currFrame;
public:
	virtual void Start(Sprite* s, Animation* a);
	FrameRangeAnimator(void);
	~FrameRangeAnimator();
	void FrameRangeAnimator::Progress();
};
