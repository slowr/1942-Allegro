#include "SmallEnemyExplosion.h"
#include "LatelyDestroyable.h"

void OnAnimationFinish(Animator *a, void *d){
	LatelyDestroyable::Add(new DestroyableObject(a, (Sprite *)d));
}

SmallEnemyExplosion::SmallEnemyExplosion(int x, int y) : 
Sprite(x, y, AnimationFilmHolder::Get().GetFilm("enemy.explosion_small"), spritetype_t::EXPLOSION){
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

