#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "MovingAnimator.h"
#include "AnimatorHolder.h"
#include "SpriteHolder.h"

class PlayerBullet : public Sprite {
	MovingAnimator * animator;
	MovingAnimation * animation;
	const static int speed = 20;
	const static int delay = 50;
public:
	const static int MAX_BULLETS = 10;
	static timestamp_t last_timestamp;
	PlayerBullet(void);
	void FireBullet(Point p, timestamp_t curr_timestamp);
	void StopBullet();
	static void FireBullets(PlayerBullet b[], Point p, timestamp_t curr_timestamp);
	virtual void PlayerBullet::Draw(ALLEGRO_BITMAP *dest);
	virtual void CollisionResult(Sprite *s);
	virtual void AnimationFinish(void);
	~PlayerBullet(void);
};