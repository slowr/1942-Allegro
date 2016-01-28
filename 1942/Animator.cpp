#include "Animator.h"

void Animator::NotifyStopped(void) {
	if (onFinish)
		(*onFinish)(this, finishClosure);
}

bool Animator::HasFinished(void) const{
	return state != ANIMATOR_RUNNING;
}

void Animator::SetOnFinish(FinishCallback f, void* c){
	onFinish = f, finishClosure = c;
}

Animator::~Animator() {};

void Animator::Stop(void) {
	if (!HasFinished()) {
		state = ANIMATOR_STOPPED;
		NotifyStopped();
	}
}

void Animator::TimeShift(timestamp_t offset){
	lastTime += offset;
}

void Animator::TimeSet(timestamp_t value){
	lastTime = value;
}

Animator::Animator(void) :
	lastTime(0), state(ANIMATOR_FINISHED),
	onFinish((FinishCallback)0), finishClosure((void*)0) {}