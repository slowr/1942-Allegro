#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "MovingAnimation.h"
#include "MovingAnimator.h"
#include "SpriteHolder.h"
#include "PlayerBullet.h"

class Enemy : public Sprite {
	MovingAnimator * animator;
	MovingAnimation * animation;
	timestamp_t last_timestamp;
	const static int speed = 5;
	const static int delay = 50;
public:
	Enemy(void);
	virtual void CollisionResult(spritetype_t type);
};