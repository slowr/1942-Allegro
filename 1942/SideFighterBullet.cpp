#include "SideFighterBullet.h"
#include "AnimationFilmHolder.h"
#include "MovingAnimation.h"
#include "MovingAnimator.h"
#include "AnimatorHolder.h"
#include "LatelyDestroyable.h"

SideFighterBullet::SideFighterBullet(float _x, float _y) : Sprite(_x, _y, AnimationFilmHolder::Get().GetFilm("single.bullet"), spritetype_t::PLAYER_BULLET)
{
	animation = new MovingAnimation(0, -speed, delay, true, 1);
	animator = new MovingAnimator();
	animator->Start(this, animation);
	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
}

SideFighterBullet::~SideFighterBullet() {
	std::cout << "Side fighter bullet destroyed" << std::endl;
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animation;
	delete animator;
}

void SideFighterBullet::CollisionResult(Sprite *s) {
	switch (s->GetType()) {
	case spritetype_t::ENEMY:
		state = spritestate_t::DEAD;
		break;
	}
}

void SideFighterBullet::StopBullet() {
	LatelyDestroyable::Add(this);
}

void SideFighterBullet::Draw() {
	if (y < -frameBox.h*ScaleFactor) {
		StopBullet();
	}
	else {
		currFilm->DisplayFrame(Point(x, y), frameNo);
	}
}