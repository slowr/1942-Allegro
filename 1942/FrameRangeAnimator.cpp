#include "FrameRangeAnimator.h"

void FrameRangeAnimator::Start(Sprite* s, Animation* a) {
	sprite = s;
	anim = (FrameRangeAnimation *)a;
	lastTime = TIMESTAMP(tickCount);
	state = ANIMATOR_RUNNING;
	sprite->SetFrame(currFrame = anim->GetStartFrame());
}

FrameRangeAnimator::FrameRangeAnimator(void) :
sprite((Sprite*)0),
anim((FrameRangeAnimation*)0), currFrame(0xFF) {}

FrameRangeAnimator::~FrameRangeAnimator() {}

void FrameRangeAnimator::Progress() {
	timestamp_t currTime = TIMESTAMP(tickCount);
	while (currTime > lastTime && currTime - lastTime >= anim->GetDelay()) {
		float dx = 0, dy = 0;
		if (currFrame == anim->GetEndFrame()){
			currFrame = anim->GetStartFrame();
		} else {
			if (anim->GetStartFrame() > anim->GetEndFrame()){
				--currFrame;
			}
			else{
				++currFrame;
			}
		}
		
		dx = sprite->GetFrameBox(sprite->GetFrame()).w / 2;
		dy = sprite->GetFrameBox(sprite->GetFrame()).h / 2;
		sprite->SetFrame(currFrame);
		dx -= sprite->GetFrameBox(sprite->GetFrame()).w / 2;
		dy -= sprite->GetFrameBox(sprite->GetFrame()).h / 2;

		sprite->Move(anim->GetDx() + dx, anim->GetDy() + dy);
		lastTime += anim->GetDelay();

		if (currFrame == anim->GetEndFrame() && !anim->GetContinuous()) {
			state = ANIMATOR_FINISHED;
			NotifyStopped();
			return;
		}
	}
}