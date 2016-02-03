#include "SmallEnemyExplosion.h"
#include "LatelyDestroyable.h"

SmallEnemyExplosion::SmallEnemyExplosion(int x, int y) : 
Sprite(x, y, AnimationFilmHolder::Get().GetFilm("enemy.explosion"), spritetype_t::EXPLOSION){
	animation = new FrameRangeAnimation(0, 5, 0, 0, delay, false, 2);
	animator = new FrameRangeAnimator();
	animator->Start(this, animation, TIMESTAMP(tickCount));
	animator->SetOnFinish(OnAnimationFinish, this);
	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
}

void SmallEnemyExplosion::CollisionResult(spritetype_t type){
	return;
}

SmallEnemyExplosion::~SmallEnemyExplosion(){
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}

void SmallEnemyExplosion::AnimationFinish(){
	LatelyDestroyable::Add(this);
}

