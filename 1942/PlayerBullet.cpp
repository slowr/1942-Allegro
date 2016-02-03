#include "PlayerBullet.h"

timestamp_t PlayerBullet::last_timestamp = 0;

PlayerBullet::PlayerBullet(void) : Sprite(0, 0, AnimationFilmHolder::Get().GetFilm("player.bullet"), spritetype_t::PLAYER_BULLET) {
	animation = new MovingAnimation(0, -speed, delay, true, 1);
	animator = new MovingAnimator();
	animator->Start(this, animation, 0);
	AnimatorHolder::Register(animator);
	isVisible = false;
	state = spritestate_t::WAIT;
}

void PlayerBullet::FireBullet(Point p, timestamp_t curr_timestamp){
	animator->TimeSet(curr_timestamp);
	state = spritestate_t::ALIVE;
	x = p.x + (AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).w * ScaleFactor) / 2 - (AnimationFilmHolder::Get().GetFilm("player.bullet")->GetFrameBox(0).w*ScaleFactor) / 2;
	y = p.y - (AnimationFilmHolder::Get().GetFilm("player.bullet")->GetFrameBox(0).w*ScaleFactor) / 2;
	AnimatorHolder::MarkAsRunning(animator);
	isVisible = true;
}

void PlayerBullet::StopBullet(){
	isVisible = false;
	state = spritestate_t::WAIT;
	AnimatorHolder::MarkAsSuspended(animator);
}

void PlayerBullet::FireBullets(PlayerBullet b[], Point p, timestamp_t curr_timestamp){
	if (curr_timestamp - last_timestamp > 250){
		for (int i = 0; i < MAX_BULLETS; i++){
			if (!b[i].isVisible){
				b[i].FireBullet(p, curr_timestamp);
				break;
			}
		}
		last_timestamp = curr_timestamp;
	}
}

void PlayerBullet::Draw(ALLEGRO_BITMAP *dest){
	if (y < -frameBox.h) PlayerBullet::StopBullet();
	else currFilm->DisplayFrame(dest, Point(x, y), frameNo);
}

void PlayerBullet::CollisionResult(Sprite *s){
	switch (s->GetType()){
	case spritetype_t::ENEMY:
		StopBullet();
		break;
	}
}

void PlayerBullet::AnimationFinish(void){ }

PlayerBullet::~PlayerBullet(){
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}