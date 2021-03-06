#pragma once

#include "types.h"
#include "Sprite.h"
#include "Animation.h"

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
	void NotifyStopped(void);
public:
	virtual void Start(Sprite* s, Animation* a) = 0;
	bool HasFinished(void) const;
	virtual void Progress() = 0;
	void SetOnFinish(FinishCallback f, void* c = (void*)0);
	virtual ~Animator();
	void Stop(void);
	void TimeShift(timestamp_t offset);
	void TimeSet(timestamp_t value);

	Animator(void);
};