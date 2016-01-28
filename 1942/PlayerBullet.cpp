#include "PlayerBullet.h"

timestamp_t PlayerBullet::last_timestamp = 0;

void PlayerBullet::BulletCollisionHandler::operator()(Sprite *caller, Sprite *arg) {
	if (arg->GetType() == spritetype_t::ENEMY){
		std::cout << "bullet hit by enemy" << std::endl;
	}
}

PlayerBullet::PlayerBullet(void) : Sprite(0, 0, AnimationFilmHolder::Get().GetFilm("player.bullet"), spritetype_t::PLAYER_BULLET) {
	AddCollisionHandler(new BulletCollisionHandler());
	animation = new MovingAnimation(0, -speed, delay, true, 1);
	animator = new MovingAnimator();
	animator->Start(this, animation, 0);
	AnimatorHolder::Register(animator);
	isVisible = false;
	SpriteHolder::Get().Add(this);
}

void PlayerBullet::FireBullet(Point p, timestamp_t curr_timestamp){
	animator->TimeSet(curr_timestamp);
	x = p.x + AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).w / 4;
	y = p.y - AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).h / 4;
	AnimatorHolder::MarkAsRunning(animator);
	isVisible = true;
}

void PlayerBullet::Draw(ALLEGRO_BITMAP *dest){
	AnimationFilmHolder::Get().GetFilm("player.bullet")->DisplayFrame(dest, Point(x, y), frameNo);
}

void PlayerBullet::StopBullet(){
	isVisible = false;
	AnimatorHolder::MarkAsSuspended(animator);
}

void PlayerBullet::FireBullets(PlayerBullet b[], Point p, timestamp_t curr_timestamp){
	if (curr_timestamp - last_timestamp > 50){
		for (int i = 0; i < MAX_BULLETS; i++){
			if (!b[i].isVisible){
				b[i].FireBullet(p, curr_timestamp);
				break;
			}
		}
		last_timestamp = curr_timestamp;
	}
}

void PlayerBullet::DrawBullets(PlayerBullet b[], ALLEGRO_BITMAP *dest){
	for (int i = 0; i < MAX_BULLETS; i++){
		if (b[i].isVisible){
			if (b[i].y < 0)
				b[i].StopBullet();
			else
				b[i].Draw(dest);
		}
	}
}

