#include "MovingPathAnimator.h"
#include "Enemy.h"
void MovingPathAnimator::Start(Sprite* s, Animation* a, timestamp_t t) {
	sprite = s;
	anim = (MovingPathAnimation *)a;
	lastTime = t;
	state = ANIMATOR_RUNNING;
}

MovingPathAnimator::MovingPathAnimator(void) :
sprite((Sprite*)0), anim((MovingPathAnimation*)0) {}

MovingPathAnimator::~MovingPathAnimator() {}

void MovingPathAnimator::Progress(timestamp_t currTime) {
	while (!anim->GetPath().empty() && currTime > lastTime && currTime - lastTime >= anim->GetPath().front()->delay) {
		PathEntry *p = anim->GetPath().front();
		float dx = 0, dy = 0;

		dx = sprite->GetFrameBox(sprite->GetFrame()).w / 2;
		dy = sprite->GetFrameBox(sprite->GetFrame()).h / 2;
		sprite->SetFrame(p->frame);
		dx -= sprite->GetFrameBox(sprite->GetFrame()).w / 2;
		dy -= sprite->GetFrameBox(sprite->GetFrame()).h / 2;

		sprite->Move(p->dx + dx, p->dy + dy);

		--p->repetitions;

		if ((sprite->GetType() == ENEMY) && ((Enemy *)sprite)->GetSubType() == GREEN_LARGE){
			if (p->action == SHOOT){
				switch (p->repetitions){
				case 5:
					((Enemy *)sprite)->shoot(TIMESTAMP(tickCount));
					break;
				case 10:
					((Enemy *)sprite)->shoot(TIMESTAMP(tickCount));
					break;
				case 15:
					((Enemy *)sprite)->shoot(TIMESTAMP(tickCount));
					break;
				case 20:
					((Enemy *)sprite)->shoot(TIMESTAMP(tickCount));
					break;
				}
			}
		}

		if (p->repetitions == 0) {
			if (p->action == SHOOT){
				if (sprite->GetType() == ENEMY){
					((Enemy *)sprite)->shoot(TIMESTAMP(tickCount));
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
