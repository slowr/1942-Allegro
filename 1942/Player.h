#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "MovingPathAnimator.h"
#include "SpriteHolder.h"
#include "LatelyDestroyable.h"
#include "SideFighter.h"

enum playermovement_t {
	LEFT, RIGHT, NONE, LOOP, TAKEOFF, LANDING, LANDED
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
	MovingPathAnimation *landingAnimation;
	MovingPathAnimator *landingAnimator;
	timestamp_t last_timestamp;
	playermovement_t movement;
	bool dead;
	bool hasSideFighters;
	std::list<PathEntry *> loopList;
	std::list<PathEntry *> takeoffList;
	std::list<PathEntry *> landingList;
	std::list<SideFighter *> sideFightersList;
	const static int delay = 150;
	const static int move_delay = 15;
	const static int speed = 7;
	static void movementAnimatorCallback(Animator *, void *);
	static void reverseAnimatorCallback(Animator *, void *);
	static void loopAnimatorCallback(Animator *, void *);
	static void takeoffAnimatorCallback(Animator *a, void *);
	static void landingAnimatorCallback(Animator *a, void *c);
public:
	Player(void);
	void Move(bool up, bool down, bool left, bool right);
	const Point getPos() const;
	virtual void CollisionResult(Sprite *s);
	virtual void AnimationFinish(void);
	void TakeOff();
	void Loop();
	void Land();
	void MoveLeft();
	void MoveRight();
	void StopMoving();
	void Explode();
	bool isDead();
	void shoot();
	void Move(offset_t dx, offset_t dy);
	void setDead(bool val);
	void LostSideFighter(SideFighter *s);
	void SetSideFighters(bool val);
	playermovement_t GetMovement();
	~Player(void);
};