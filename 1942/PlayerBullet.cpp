#include "PlayerBullet.h"

timestamp_t PlayerBullet::last_timestamp = 0;

PlayerBullet::PlayerBullet(void) : Sprite(0, 0, AnimationFilmHolder::Get().GetFilm("player.bullet"), spritetype_t::PLAYER_BULLET) {
	animation = new MovingAnimation(0, -speed, delay, true, 1);
	animator = new MovingAnimator();
	animator->Start(this, animation);
	AnimatorHolder::Register(animator);
	isVisible = false;
	state = spritestate_t::WAIT;
}

void PlayerBullet::FireBullet(Point p){
	animator->TimeSet(tickCount);
	state = spritestate_t::ALIVE;
	x = p.x + (AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).w * ScaleFactor) / 2 - (AnimationFilmHolder::Get().GetFilm("player.bullet")->GetFrameBox(0).w*ScaleFactor) / 2;
	y = p.y - (AnimationFilmHolder::Get().GetFilm("player.bullet")->GetFrameBox(0).w * ScaleFactor) / 2;
	AnimatorHolder::MarkAsRunning(animator);
	isVisible = true;
}

void PlayerBullet::StopBullet(){
	isVisible = false;
	state = spritestate_t::WAIT;
	AnimatorHolder::MarkAsSuspended(animator);
}

void PlayerBullet::FireBullets(PlayerBullet b[], Point p){
	timestamp_t currTime = TIMESTAMP(tickCount);
	if (currTime - last_timestamp > 250){
		for (int i = 0; i < MAX_BULLETS; i++){
			if (!b[i].isVisible){
				b[i].FireBullet(p);
				break;
			}
		}
		last_timestamp = currTime;
	}
}

void PlayerBullet::Draw(){
	if (y < -frameBox.h*ScaleFactor) PlayerBullet::StopBullet();
	else currFilm->DisplayFrame(Point(x, y), frameNo);
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