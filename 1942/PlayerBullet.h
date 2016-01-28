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
	const static int delay = 50;
public:
	const static int MAX_BULLETS = 10;
	static timestamp_t last_timestamp;

	class BulletCollisionHandler : public Sprite::CollisionHandler {
	public:
		virtual void operator()(Sprite *caller, Sprite *arg);
	};

	PlayerBullet(void);
	void FireBullet(Point p, timestamp_t curr_timestamp);
	void Draw(ALLEGRO_BITMAP *dest);
	void StopBullet();
	static void FireBullets(PlayerBullet b[], Point p, timestamp_t curr_timestamp);
	static void DrawBullets(PlayerBullet b[], ALLEGRO_BITMAP *dest);
};