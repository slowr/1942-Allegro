#include "MovingPathAnimator.h"

void MovingPathAnimator::Start(Sprite* s, MovingPathAnimation* a, timestamp_t t) {
	sprite = s;
	anim = a;
	lastTime = t;
	state = ANIMATOR_RUNNING;
}

MovingPathAnimator::MovingPathAnimator(void) :
sprite((Sprite*)0), anim((MovingPathAnimation*)0) {}

MovingPathAnimator::~MovingPathAnimator() {}

void MovingPathAnimator::Progress(timestamp_t currTime) {
	while (!anim->GetPath().empty() && currTime > lastTime && currTime - lastTime >= anim->GetPath().front()->delay) {
		PathEntry *p = anim->GetPath().front();

		sprite->Move(p->dx, p->dy);
		sprite->SetFrame(p->frame);

		if (--p->repetitions == 0) {
			anim->GetPath().pop_front();
		}

		lastTime += p->delay;

		if (anim->GetPath().empty()) {
			std::cout << "ADSDSADSADSASD" << std::endl;
			state = ANIMATOR_FINISHED;
			NotifyStopped();
			return;
		}
	}
}
