#include "LargeEnemyExplosion.h"
#include "LatelyDestroyable.h"


LargeEnemyExplosion::LargeEnemyExplosion(float x, float y) : Sprite(x, y, AnimationFilmHolder::Get().GetFilm("large.explosion"), spritetype_t::EXPLOSION)
{
	animation = new FrameRangeAnimation(0, 10, 0, 0, delay, false, 2);
	animator = new FrameRangeAnimator();
	animator->Start(this, animation);
	animator->SetOnFinish(OnAnimationFinish, this);
	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
}


LargeEnemyExplosion::~LargeEnemyExplosion()
{
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}

void LargeEnemyExplosion::CollisionResult(spritetype_t type) {
	return;
}

void LargeEnemyExplosion::AnimationFinish() {
	LatelyDestroyable::Add(this);
}