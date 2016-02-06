#include "MovingPathAnimator.h"
#include "Enemy.h"
void MovingPathAnimator::Start(Sprite* s, Animation* a) {
	sprite = s;
	anim = (MovingPathAnimation *)a;
	lastTime = TIMESTAMP(tickCount);
	state = ANIMATOR_RUNNING;
}

MovingPathAnimator::MovingPathAnimator(void) :
sprite((Sprite*)0), anim((MovingPathAnimation*)0) {}

MovingPathAnimator::~MovingPathAnimator() {}

void MovingPathAnimator::Progress() {
	timestamp_t currTime = TIMESTAMP(tickCount);
	while (!anim->GetPath().empty() && currTime > lastTime && currTime - lastTime >= anim->GetPath().front()->delay) {
		PathEntry *p = anim->GetPath().front();
		float dx = 0, dy = 0;

		dx = sprite->GetFrameBox(sprite->GetFrame()).w / 2;
		dy = sprite->GetFrameBox(sprite->GetFrame()).h / 2;
		sprite->SetFrame(p->frame);
		dx -= sprite->GetFrameBox(sprite->GetFrame()).w / 2;
		dy -= sprite->GetFrameBox(sprite->GetFrame()).h / 2;

		sprite->Move(p->dx + dx, p->dy + dy);

		if (--p->repetitions == 0) {
			if (p->action == SHOOT){
				if (sprite->GetType() == ENEMY){
					((Enemy *)sprite)->shoot();
				}
			}
			anim->GetPath().remove(p);
		}

		lastTime += p->delay;

		if (anim->GetPath().empty()) {
			state = ANIMATOR_FINISHED;
			NotifyStopped();
			return;
		}
	}
}
