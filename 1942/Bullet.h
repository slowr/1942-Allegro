#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "MovingAnimator.h"
#include "AnimatorHolder.h"

class Bullet : public Sprite {
	MovingAnimator * animator;
	MovingAnimation * animation;
	const static int speed = 5;
	const static int delay = 50;
public:
	const static int MAX_BULLETS = 10;
	static timestamp_t last_timestamp;

	class BulletCollisionHandler : public Sprite::CollisionHandler {
	public:
		virtual void operator()(Sprite *caller, Sprite *arg) {

		}
	};

	Bullet(void) : Sprite(0, 0, AnimationFilmHolder::Get().GetFilm("player.bullet"), spritetype_t::PLAYER_BULLET){
		BulletCollisionHandler *b = new BulletCollisionHandler();
		AddCollisionHandler(b);
		animation = new MovingAnimation(0, -speed, delay, true, 1);
		animator = new MovingAnimator();
		animator->Start(this, animation, 0);
		AnimatorHolder::Register(animator);
		isVisible = false;
	}

	void FireBullet(Point p, timestamp_t curr_timestamp){
		animator->TimeSet(curr_timestamp);
		x = p.x + AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).w / 4;
		y = p.y - AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).h / 4;
		AnimatorHolder::MarkAsRunning(animator);
		isVisible = true;
	}

	void Draw(ALLEGRO_BITMAP *dest){
		AnimationFilmHolder::Get().GetFilm("player.bullet")->DisplayFrame(dest, Point(x, y), frameNo);
	}

	void StopBullet(){
		isVisible = false;
		AnimatorHolder::MarkAsSuspended(animator);
	}

	static void FireBullets(Bullet b[], Point p, timestamp_t curr_timestamp){
		if (curr_timestamp - last_timestamp > 50){
			for (int i = 0; i < MAX_BULLETS; i++){
				if (!b[i].isVisible){
					b[i].FireBullet(p,curr_timestamp);
					break;
				}
			}
			last_timestamp = curr_timestamp;
		}
	}

	static void DrawBullets(Bullet b[], ALLEGRO_BITMAP *dest){
		for (int i = 0; i < MAX_BULLETS; i++){
			if (b[i].isVisible){
				if (b[i].y < 0)
					b[i].StopBullet();
				else
					b[i].Draw(dest);
			}
		}
	}
};