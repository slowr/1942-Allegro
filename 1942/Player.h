#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "FrameRangeAnimation.h"
#include "Entity.h"

class Player : Entity {
	Sprite * sprite;
	FrameRangeAnimator * animator;
	FrameRangeAnimation * animation;
	timestamp_t last_timestamp;
public:
	const static int speed = 5;

	class PlayerCollisionHandler : public Sprite::CollisionHandler {
	public:
		virtual void operator()(Sprite *caller, Sprite *arg) {
			std::cout << "LOL" << std::endl;
		}
	};

	Player(void){
		AnimationFilm * film = AnimationFilmHolder::Get().GetFilm("player.sprite");
		PlayerCollisionHandler *p = new PlayerCollisionHandler();
		sprite = new Sprite(SCREEN_W / 2 - film->GetFrameBox(0).w / 2, SCREEN_H - film->GetFrameBox(0).h, film, spritetype_t::PLAYER);
		sprite->AddCollisionHandler(p);
		animation = new FrameRangeAnimation(0, 2, 0, 0, 200, true, 1);
		animator = new FrameRangeAnimator();
		animator->Start(sprite, animation, 0);
		AnimatorHolder::Register(animator);
		sprite->SetVisibility(true);
		last_timestamp = 0;
		setSprite(sprite);
	}

	void Move(float x, float y, timestamp_t curr_timestamp){
		if (sprite->getX() + x < 0 || sprite->getX() + x + sprite->GetFrameBox().w > SCREEN_W) return;
		if (sprite->getY() + y < 0 || sprite->getY() + y + sprite->GetFrameBox().h > SCREEN_H) return;

		if (curr_timestamp - last_timestamp > 25){
			sprite->Move(x, y);
			last_timestamp = curr_timestamp;
		}
	}

	const Point getPos() const {
		return Point(sprite->getX(), sprite->getY());
	}

	void DrawPlayer(ALLEGRO_BITMAP * dest){
		AnimationFilmHolder::Get().GetFilm("player.sprite")->DisplayFrame(dest, Point(sprite->getX(), sprite->getY()), sprite->GetFrame());
	}

};