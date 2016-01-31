#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "MovingPathAnimation.h"
#include "MovingPathAnimator.h"
#include "SpriteHolder.h"
#include "PlayerBullet.h"
#include "LatelyDestroyable.h"

class Enemy : public Sprite {
	MovingPathAnimator * animator;
	MovingPathAnimation * animation;
	timestamp_t last_timestamp;
	const static int speed = 5;
	const static int delay = 50;
public:
	Enemy(void);
	virtual void CollisionResult(spritetype_t type);
	virtual void AnimationFinish(void);
	~Enemy(void);
};