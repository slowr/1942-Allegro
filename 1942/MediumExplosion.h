#pragma once
#include "Sprite.h"
#include "AnimationFilmHolder.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "FrameRangeAnimation.h"
#include "SpriteHolder.h"

class MediumExplosion :
	public Sprite
{
	FrameRangeAnimator * animator;
	FrameRangeAnimation * animation;
	const static int delay = 200;
public:
	virtual void CollisionResult(spritetype_t type);
	virtual void AnimationFinish(void);
	MediumExplosion(float x, float y);
	~MediumExplosion();
};