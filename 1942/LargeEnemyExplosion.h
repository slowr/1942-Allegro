#pragma once
#include "Sprite.h"
#include "AnimationFilmHolder.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "FrameRangeAnimation.h"
#include "SpriteHolder.h"

class LargeEnemyExplosion :	public Sprite
{
	FrameRangeAnimator * animator;
	FrameRangeAnimation * animation;
	const static int delay = 200;
public:
	virtual void CollisionResult(spritetype_t type);
	virtual void AnimationFinish(void);
	LargeEnemyExplosion(float x, float y);
	~LargeEnemyExplosion();
};

