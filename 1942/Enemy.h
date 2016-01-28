#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "MovingAnimation.h"
#include "MovingAnimator.h"

class Enemy : public Sprite {
	MovingAnimator * animator;
	MovingAnimation * animation;
	timestamp_t last_timestamp;
	const static int speed = 5;
	const static int delay = 50;
public:
	class EnemyCollisionHandler : public CollisionHandler {
	public:
		virtual void operator()(Sprite *caller, Sprite *arg) {
			if (arg->GetType() == spritetype_t::PLAYER_BULLET){
				std::cout << "enemy hit by player bullet" << std::endl;
			}
		}
	};

	Enemy(void) : Sprite(SCREEN_W / 2 - AnimationFilmHolder::Get().GetFilm("enemy.blue_plane")->GetFrameBox(0).w / 2, 0, AnimationFilmHolder::Get().GetFilm("enemy.blue_plane"), spritetype_t::ENEMY){
		EnemyCollisionHandler *e = new EnemyCollisionHandler();
		AddCollisionHandler(e);
		animation = new MovingAnimation(0, speed, delay, true, 1);
		animator = new MovingAnimator();
		animator->Start(this, animation, 0);
		AnimatorHolder::Register(animator);
		AnimatorHolder::MarkAsRunning(animator);
	}

	void Draw(ALLEGRO_BITMAP *dest){
		AnimationFilmHolder::Get().GetFilm("enemy.blue_plane")->DisplayFrame(dest, Point(x, y), frameNo);
	}
};