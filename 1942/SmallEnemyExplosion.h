#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "FrameRangeAnimation.h"
#include "SpriteHolder.h"

class SmallEnemyExplosion : public Sprite {
	FrameRangeAnimator * animator;
	FrameRangeAnimation * animation;
	const static int delay = 200;
public:
	SmallEnemyExplosion(int x, int y);
	virtual void CollisionResult(spritetype_t type);
	virtual void AnimationFinish(void);
	~SmallEnemyExplosion(void);
};