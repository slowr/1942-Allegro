#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "FrameRangeAnimation.h"
#include "SpriteHolder.h"

class PlayerExplosion : public Sprite {
	FrameRangeAnimator * animator;
	FrameRangeAnimation * animation;
	const static int delay = 200;
public:
	PlayerExplosion(int x, int y);
	virtual void CollisionResult(spritetype_t type);
	virtual void AnimationFinish(void);
	~PlayerExplosion(void);
};