#pragma once
#include "Sprite.h"
#include "MovingPathAnimation.h"
#include "MovingPathAnimator.h"

class SideFighter : public Sprite
{
	MovingPathAnimator *animator;
	MovingPathAnimation *animation;
	bool ready;
public:
	SideFighter(float _x, float _y, float offsetX, float offsetY);
	virtual ~SideFighter();
	void shoot();
	void Explode();
	void Move(float dx, float dy);
	static void entryAnimatorCallback(Animator *a, void *c);
	virtual void CollisionResult(Sprite *s);
};