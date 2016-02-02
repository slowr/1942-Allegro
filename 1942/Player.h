#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "FrameRangeAnimation.h"
#include "SpriteHolder.h"
#include "LatelyDestroyable.h"

enum playermovement_t {
	LEFT, RIGHT, NONE
};

class Player : public Sprite {
	FrameRangeAnimator * leftAnimator;
	FrameRangeAnimation * leftAnimation;
	FrameRangeAnimator * rightAnimator;
	FrameRangeAnimation * rightAnimation;
	FrameRangeAnimator * tumbleAnimator;
	FrameRangeAnimation * tumbleAnimation;
	timestamp_t last_timestamp;
	playermovement_t movement;
	const static int delay = 200;
	unsigned lifes = 3;

	static void movementAnimatorCallback(Animator *, void *);
public:
	const static int speed = 5;
	Player(void);
	void Move(bool up, bool down, bool left, bool right, timestamp_t curr_timestamp);
	const Point getPos() const;
	virtual void CollisionResult(spritetype_t type);
	virtual void AnimationFinish(void);
	void Tumble();
	void MoveLeft();
	void MoveRight();
	void StopMoving();
	~Player(void);
};