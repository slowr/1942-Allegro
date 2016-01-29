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
	x = p.x + AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).w / 4;
	y = p.y - AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).h / 4;
	AnimatorHolder::MarkAsRunning(animator);
	isVisible = true;
}

void PlayerBullet::StopBullet(){
	isVisible = false;
	state = spritestate_t::WAIT;
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

void PlayerBullet::Draw(ALLEGRO_BITMAP *dest){
	if (y < 0) PlayerBullet::StopBullet();
	else currFilm->DisplayFrame(dest, Point(x, y), frameNo);
}

void PlayerBullet::CollisionResult(spritetype_t type){
	switch (type){
	case spritetype_t::ENEMY:
		StopBullet();
		break;
	}
}

