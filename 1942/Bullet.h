#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "MovingAnimator.h"
#include "AnimatorHolder.h"

class Bullet {
	Sprite * sprite;
	MovingAnimator * animator;
	MovingAnimation * animation;
public:
	const static int MAX_BULLETS = 10;
	const static int speed = 10;
	static timestamp_t last_timestamp;

	Bullet(void){
		sprite = new Sprite(0, 0, AnimationFilmHolder::Get().GetFilm("player.bullet"));
		animation = new MovingAnimation(0, -speed, 50, true, 1);
		animator = new MovingAnimator();
		animator->Start(sprite, animation, 0);
		AnimatorHolder::Register(animator);
		sprite->SetVisibility(false);
	}

	static void FireBullet(Bullet b[], Point p, timestamp_t curr_timestamp){
		if (curr_timestamp - last_timestamp > 500){
			for (int i = 0; i < MAX_BULLETS; i++){
				if (!b[i].sprite->IsVisible()){
					//std::cout << "bullet #" << i << b[i].bullet_sprite->getY() << std::endl;
					b[i].animator->TimeSet(curr_timestamp);
					b[i].sprite->setX(p.x + AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).w/4);
					b[i].sprite->setY(p.y - AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).h/4);
					AnimatorHolder::MarkAsRunning(b[i].animator);
					b[i].sprite->SetVisibility(true);
					break;
				}
			}
			last_timestamp = curr_timestamp;
		}
	}

	static void DrawBullets(Bullet b[], ALLEGRO_BITMAP *dest){
		for (int i = 0; i < MAX_BULLETS; i++){
			if (b[i].sprite->IsVisible()){
				if (b[i].sprite->getY() < 0)
					b[i].StopBullet();
				else
					AnimationFilmHolder::Get().GetFilm("player.bullet")->DisplayFrame(dest, Point(b[i].sprite->getX(), b[i].sprite->getY()), b[i].sprite->GetFrame());
			}
		}
	}

	void StopBullet(){
		sprite->SetVisibility(false);
		AnimatorHolder::MarkAsSuspended(animator);
	}
};