#include "SideFighter.h"
#include "AnimationFilmHolder.h"
#include "SmallEnemyExplosion.h"
#include "SideFighterBullet.h"
#include "GameController.h"
#include "MovingPathAnimator.h"
#include "MovingPathAnimation.h"

void SideFighter::entryAnimatorCallback(Animator *a, void *c) {
	LatelyDestroyable::Add(a);
	((SideFighter *)c)->ready = true;
}

SideFighter::SideFighter(float _x, float _y, float offsetX, float offsetY) : Sprite(_x, _y, AnimationFilmHolder::Get().GetFilm("green.jet"), spritetype_t::PLAYER), ready(false)
{
	std::list<PathEntry *> list;
	PathEntry *pE = new PathEntry();

	float dx = GameController::Get().getPlayer()->GetX() - x + offsetX;
	float dy = GameController::Get().getPlayer()->GetY() - y + offsetY;
	float distance = sqrt(pow(dx, 2) + pow(dy, 2));
	
	pE->delay = 25;
	pE->action = RANDOM_FRAME;
	pE->frame = 16;	// random frames from 0 to 15, last repetition will set it to 0

	int repetitions = distance / 20;
	pE->dx = dx / repetitions;
	pE->dy = dy / repetitions;
	pE->repetitions = repetitions;
	
	list.push_back(pE);

	animation = new MovingPathAnimation(std::list<PathEntry *>(list), 2337);
	animator = new MovingPathAnimator();
	animator->SetOnFinish(entryAnimatorCallback, this);
	animator->Start(this, animation);

	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
}

SideFighter::~SideFighter()
{
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}

void SideFighter::shoot() {
	if (!ready) return;
	float _x = x + (GetFrameBox(0).w * ScaleFactor) / 2;
	float _y = y - 5;
	new SideFighterBullet(_x, _y);
}

void SideFighter::Move(float dx, float dy){
	x += dx;
	y += dy;
}

void SideFighter::Explode() {
	GameController::Get().getPlayer()->LostSideFighter(this);
	state = spritestate_t::DEAD;
	new SmallEnemyExplosion(x, y);
}

void SideFighter::CollisionResult(Sprite *s) {
	switch (s->GetType()) {
	case spritetype_t::ENEMY:
	case spritetype_t::ENEMY_BULLET:
		if (!ready) return;
		Explode();
		break;
	}
}