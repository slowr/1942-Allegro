#include "FrameRangeAnimator.h"

void FrameRangeAnimator::Start(Sprite* s, Animation* a, timestamp_t t) {
	sprite = s;
	anim = (FrameRangeAnimation *)a;
	lastTime = t;
	state = ANIMATOR_RUNNING;
	sprite->SetFrame(currFrame = anim->GetStartFrame());
}

FrameRangeAnimator::FrameRangeAnimator(void) :
sprite((Sprite*)0),
anim((FrameRangeAnimation*)0), currFrame(0xFF) {}

FrameRangeAnimator::~FrameRangeAnimator() {}

void FrameRangeAnimator::Progress(timestamp_t currTime) {
	while (currTime > lastTime && currTime - lastTime >= anim->GetDelay()) {
		if (currFrame == anim->GetEndFrame()){
			std::cout << "LOL" << std::endl;
			currFrame = anim->GetStartFrame();
		} else {
			if (anim->GetStartFrame() > anim->GetEndFrame())
				--currFrame;
			else
				++currFrame;
		}
		sprite->Move(anim->GetDx(), anim->GetDy());
		sprite->SetFrame(currFrame);
		lastTime += anim->GetDelay();

		if (currFrame == anim->GetEndFrame() && !anim->GetContinuous()) {
			state = ANIMATOR_FINISHED;
			NotifyStopped();
			return;
		}
	}
}