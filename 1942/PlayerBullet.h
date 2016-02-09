#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "MovingAnimator.h"
#include "AnimatorHolder.h"
#include "SpriteHolder.h"

class PlayerBullet : public Sprite {
	MovingAnimator * animator;
	MovingAnimation * animation;
	const static int speed = 10;
	const static int delay = 15;
	static PlayerBullet *bullets;
	static bool quadBullets;
public:
	const static int MAX_BULLETS = 10;
	static timestamp_t last_timestamp;
	PlayerBullet(void);
	void FireBullet(Point p);
	void StopBullet();
	static void InitBullets();
	static void SetQuadBullets(bool val);
	static bool FireBullets(Point p);
	virtual void PlayerBullet::Draw();
	virtual void CollisionResult(Sprite *s);
	virtual void AnimationFinish(void);
	~PlayerBullet(void);
};