#include "MovingAnimator.h"

void MovingAnimator::Start(Sprite* s, Animation* a, timestamp_t t) {
	sprite = s;
	anim = (MovingAnimation *) a;
	lastTime = t;
	state = ANIMATOR_RUNNING;
}

MovingAnimator::MovingAnimator(void) :
sprite((Sprite*)0), anim((MovingAnimation*)0) {}

MovingAnimator::~MovingAnimator() {}

void MovingAnimator::Progress(timestamp_t currTime) {
	while (currTime > lastTime && currTime - lastTime >= anim->GetDelay()) {
		sprite->Move(anim->GetDx(), anim->GetDy());
		if (!anim->GetContinuous()) {
			state = ANIMATOR_FINISHED;
			NotifyStopped();
		}
		else {
			lastTime += anim->GetDelay();
		}
	}
}
