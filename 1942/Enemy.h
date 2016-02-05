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
#define _USE_MATH_DEFINES
#include <math.h>
#include "Player.h"
#include "GameController.h"

class Enemy : public Sprite {
	enemysubtype_t subtype;
	Animator * animator;
	Animation * animation;
	timestamp_t last_timestamp;
	int health;
	const static int speed = 5;
	const static int delay = 15;
	int getFrame(float degrees);

	enum enemylook_t {
		RIGHT = 270,
		BOTTOM = 360,
		LEFT = 90,
		TOP = 180
	};
public:
	enemysubtype_t GetSubType();
	void AnimationInit();
	Enemy(float _x, float _y, std::string sprite, enemysubtype_t t);
	void doCircle(Enemy::enemylook_t direction, std::list<PathEntry*>& p, float radius);
	virtual void CollisionResult(Sprite *s);
	virtual void AnimationFinish(void);
	void shoot();
	~Enemy(void);
};