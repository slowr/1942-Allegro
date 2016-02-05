#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "MovingPathAnimator.h"
#include "SpriteHolder.h"
#include "LatelyDestroyable.h"

enum playermovement_t {
	LEFT, RIGHT, NONE, TUMBLE
};

class Player : public Sprite {
	FrameRangeAnimator * leftAnimator;
	FrameRangeAnimation * leftAnimation;
	FrameRangeAnimator * rightAnimator;
	FrameRangeAnimation * rightAnimation;
	FrameRangeAnimator * revleftAnimator;
	FrameRangeAnimation * revleftAnimation;
	FrameRangeAnimator * revrightAnimator;
	FrameRangeAnimation * revrightAnimation;
	MovingPathAnimator * tumbleAnimator;
	MovingPathAnimation * tumbleAnimation;
	timestamp_t last_timestamp;
	playermovement_t movement;
	std::list<PathEntry *> tumbleList;
	const static int delay = 15;
	unsigned lifes = 3;

	static void movementAnimatorCallback(Animator *, void *);
	static void reverseAnimatorCallback(Animator *, void *);
	static void tumbleAnimatorCallback(Animator *, void *);
public:
	const static int speed = 5;
	Player(void);
	void Move(bool up, bool down, bool left, bool right, timestamp_t curr_timestamp);
	const Point getPos() const;
	virtual void CollisionResult(Sprite *s);
	virtual void AnimationFinish(void);
	void Tumble();
	void MoveLeft();
	void MoveRight();
	void StopMoving();
	playermovement_t GetMovement();
	~Player(void);
};