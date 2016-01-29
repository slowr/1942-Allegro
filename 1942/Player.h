#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "FrameRangeAnimation.h"
#include "SpriteHolder.h"

class Player : public Sprite {
	FrameRangeAnimator * animator;
	FrameRangeAnimation * animation;
	timestamp_t last_timestamp;
	const static int delay = 200;
public:
	const static int speed = 5;
	Player(void);
	void Move(bool up, bool down, bool left, bool right, timestamp_t curr_timestamp);
	const Point getPos() const;
	virtual void CollisionResult(spritetype_t type);
};