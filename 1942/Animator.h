#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#pragma once

#include "types.h"
#include "Sprite.h"

enum animatorstate_t {
	ANIMATOR_FINISHED = 0,
	ANIMATOR_RUNNING = 1,
	ANIMATOR_STOPPED = 2
};

class Animator {
public:
	typedef void(*FinishCallback)(Animator*, void*);
protected:
	timestamp_t lastTime;
	animatorstate_t state;
	FinishCallback onFinish;
	void* finishClosure;
	void NotifyStopped(void) {
		if (onFinish)
			(*onFinish)(this, finishClosure);
	}
public:
	bool HasFinished(void) const
	{
		return state != ANIMATOR_RUNNING;
	}

	virtual void Progress(timestamp_t currTime) = 0;

	void SetOnFinish(FinishCallback f, void* c = (void*)0)
	{
		onFinish = f, finishClosure = c;
	}
	virtual ~Animator() {};

	void Stop(void) {
		if (!HasFinished()) {
			state = ANIMATOR_STOPPED;
			NotifyStopped();
		}
	}
	
	void TimeShift(timestamp_t offset)
	{
		lastTime += offset;
	}

	void TimeSet(timestamp_t value)
	{
		lastTime = value;
	}

	Animator::Animator(void) :
		lastTime(0), state(ANIMATOR_FINISHED),
		onFinish((FinishCallback)0), finishClosure((void*)0) {}
};

#endif