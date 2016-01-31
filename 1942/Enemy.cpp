#include "Enemy.h"
#include <math.h>

Enemy::Enemy(void) : Sprite(0, 0, AnimationFilmHolder::Get().GetFilm("enemy.blue_plane"), spritetype_t::ENEMY){
	std::list<PathEntry *> p;
	struct PathEntry *entry1 = new PathEntry();
	entry1->dx = 0;
	entry1->dy = 0;
	entry1->frame = 1;
	entry1->delay = 10;
	entry1->repetitions = 1;
	p.push_back(entry1);
	int x_0 = 0, y_0 = SCREEN_H / 2;
	int oldDx = 0, oldDy = 0;
	for (float y = 1; y <= SCREEN_H / 2; y++){
		for (float x = 1; x <= SCREEN_W / 2; x++){
			if( ((float)(float) pow(x - x_0, 2) + (float)pow(y - y_0, 2)) == (float)pow(SCREEN_H / 2, 2)){
				std::cout << "x: " << x << ", y: " << y << std::endl;
				struct PathEntry *entry1 = new PathEntry();
				entry1->dx = (x - oldDx)/30;
				entry1->dy = (y - oldDy)/30;
				entry1->frame = 1;
				entry1->delay = 10;
				entry1->repetitions = 30;
				p.push_back(entry1);
				oldDx = x;
				oldDy = y;
			}
		}
	}

	animation = new MovingPathAnimation(p, 1);
	animator = new MovingPathAnimator();
	animator->Start(this, animation, 0);
	animator->SetOnFinish(OnAnimationFinish, this);
	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
}

void Enemy::CollisionResult(spritetype_t type){
	switch (type){
	case spritetype_t::PLAYER_BULLET:
	case spritetype_t::PLAYER:
		state = spritestate_t::DEAD;
		isVisible = false;
		break;
	}
}

void Enemy::AnimationFinish(void){
	//LatelyDestroyable::Add(this);
}

Enemy::~Enemy(){
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}