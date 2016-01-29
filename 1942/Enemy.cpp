#include "Enemy.h"

Enemy::Enemy(void) : Sprite(SCREEN_W / 2 - AnimationFilmHolder::Get().GetFilm("enemy.blue_plane")->GetFrameBox(0).w / 2, SCREEN_H / 2, AnimationFilmHolder::Get().GetFilm("enemy.blue_plane"), spritetype_t::ENEMY){
	animation = new MovingAnimation(0, speed, delay, true, 1);
	animator = new MovingAnimator();
	animator->Start(this, animation, 0);
	AnimatorHolder::Register(animator);
	//AnimatorHolder::MarkAsRunning(animator);
}

void Enemy::CollisionResult(spritetype_t type){
	switch (type){
	case spritetype_t::PLAYER_BULLET:
		state = spritestate_t::DEAD;
		isVisible = false;
		break;
	case spritetype_t::PLAYER:
		state = spritestate_t::DEAD;
		isVisible = false;
		break;
	}
}

Enemy::~Enemy(){
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}