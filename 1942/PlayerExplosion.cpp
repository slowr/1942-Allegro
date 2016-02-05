#include "PlayerExplosion.h"
#include "LatelyDestroyable.h"
#include "GameController.h"

PlayerExplosion::PlayerExplosion(int x, int y) :
Sprite(x, y, AnimationFilmHolder::Get().GetFilm("player.explosion"), spritetype_t::EXPLOSION){
	animation = new FrameRangeAnimation(0, 5, 0, 0, delay, false, 3);
	animator = new FrameRangeAnimator();
	animator->Start(this, animation, TIMESTAMP(tickCount));
	animator->SetOnFinish(OnAnimationFinish, this);
	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
}

void PlayerExplosion::CollisionResult(Sprite *s){
	return;
}

void PlayerExplosion::AnimationFinish(void){
	LatelyDestroyable::Add(this);
}

PlayerExplosion::~PlayerExplosion(){
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}

