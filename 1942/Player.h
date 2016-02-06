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
	MovingPathAnimation *checkPointAnimation;
	MovingPathAnimator *checkPointAnimator;
	timestamp_t last_timestamp;
	playermovement_t movement;
	std::list<PathEntry *> tumbleList;
	std::list<PathEntry *> onboardList;
	const static int delay = 150;
	const static int move_delay = 15;
	const static int speed = 7;
	bool checkPoint;
	static void movementAnimatorCallback(Animator *, void *);
	static void reverseAnimatorCallback(Animator *, void *);
	static void tumbleAnimatorCallback(Animator *, void *);
	static void checkpointAnimatorCallback(Animator *a, void *);
public:
	Player(void);
	void Move(bool up, bool down, bool left, bool right);
	const Point getPos() const;
	virtual void CollisionResult(Sprite *s);
	virtual void AnimationFinish(void);
	void CheckPointTumble();
	void SetCheckPoint(bool check);
	void Tumble();
	void MoveLeft();
	void MoveRight();
	void StopMoving();
	void Explode();
	playermovement_t GetMovement();
	~Player(void);
};