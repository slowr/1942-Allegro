#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "FrameRangeAnimation.h"

class Player : public Sprite {
	FrameRangeAnimator * animator;
	FrameRangeAnimation * animation;
	timestamp_t last_timestamp;
	const static int delay = 200;
public:
	const static int speed = 5;
	class PlayerCollisionHandler : public CollisionHandler {
	public:
		virtual void operator()(Sprite *caller, Sprite *arg) {

		}
	};

	Player(void) : Sprite(SCREEN_W / 2 - AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).w / 2, SCREEN_H - AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).h, AnimationFilmHolder::Get().GetFilm("player.sprite"), spritetype_t::PLAYER){
		PlayerCollisionHandler *p = new PlayerCollisionHandler();
		AddCollisionHandler(p);
		animation = new FrameRangeAnimation(0, 2, 0, 0, delay, true, 1);
		animator = new FrameRangeAnimator();
		animator->Start(this, animation, 0);
		AnimatorHolder::Register(animator);
		last_timestamp = 0;
	}

	void Move(bool up, bool down, bool left, bool right, timestamp_t curr_timestamp){
		int _x = 0, _y = 0;

		if (!(up || down || left || right)) return;

		if (up) _y = -speed;
		if (down) _y += speed;
		if (left) _x = -speed;
		if (right) _x += speed;
		
		if (x + _x < 0 || x + _x + frameBox.w > SCREEN_W) return;
		if (y + _y < 0 || y + _y + frameBox.h > SCREEN_H) return;

		if (curr_timestamp - last_timestamp > 25){
			x += _x;
			y += _y;
			last_timestamp = curr_timestamp;
		}
	}

	const Point getPos() const {
		return Point(x, y);
	}

	void Draw(ALLEGRO_BITMAP * dest){
		AnimationFilmHolder::Get().GetFilm("player.sprite")->DisplayFrame(dest, Point(x, y), frameNo);
	}

};