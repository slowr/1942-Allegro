#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "MovingPathAnimator.h"
#include "SpriteHolder.h"
#include "LatelyDestroyable.h"

enum playermovement_t {
	LEFT, RIGHT, NONE, LOOP, TAKEOFF, LANDING
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
	MovingPathAnimator * loopAnimator;
	MovingPathAnimation * loopAnimation;
	MovingPathAnimation *takeoffAnimation;
	MovingPathAnimator *takeoffAnimator;
	timestamp_t last_timestamp;
	playermovement_t movement;
	bool dead;
	std::list<PathEntry *> loopList;
	std::list<PathEntry *> takeoffList;
	const static int delay = 150;
	const static int move_delay = 15;
	const static int speed = 7;
	static void movementAnimatorCallback(Animator *, void *);
	static void reverseAnimatorCallback(Animator *, void *);
	static void loopAnimatorCallback(Animator *, void *);
	static void takeoffAnimatorCallback(Animator *a, void *);
public:
	Player(void);
	void Move(bool up, bool down, bool left, bool right);
	const Point getPos() const;
	virtual void CollisionResult(Sprite *s);
	virtual void AnimationFinish(void);
	void TakeOff();
	void Loop();
	void MoveLeft();
	void MoveRight();
	void StopMoving();
	void Explode();
	bool isDead();
	void shoot();
	void setDead(bool val);
	playermovement_t GetMovement();
	~Player(void);
};