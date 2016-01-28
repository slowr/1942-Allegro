#include "AnimatorHolder.h"

AnimatorList AnimatorHolder::running, AnimatorHolder::suspended;

 void AnimatorHolder::Register(Animator* a) { 
	 suspended.push_back(a); 
 }

 void AnimatorHolder::Cancel(Animator* a) { 
	 suspended.remove(a); 
 }

 void AnimatorHolder::MarkAsRunning(Animator* a){
	suspended.remove(a); running.push_back(a);
}

 void AnimatorHolder::MarkAsSuspended(Animator* a){
	running.remove(a); suspended.push_back(a);
}

void AnimatorHolder::Progress(timestamp_t currTime) {
	std::for_each(
		running.begin(), running.end(), ProgressFunctor(currTime)
		);
}

void AnimatorHolder::Pause(timestamp_t currTime) {
	std::for_each(
		running.begin(), running.end(), ProgressFunctor(currTime)
		);
}

void AnimatorHolder::PauseFunctor::operator()(Animator* a) const { 
	a->TimeSet(t); 
}

AnimatorHolder::PauseFunctor::PauseFunctor(timestamp_t _t) : t(_t) {}

void AnimatorHolder::ProgressFunctor::operator()(Animator* a) const { 
	a->Progress(t); 
}

AnimatorHolder::ProgressFunctor::ProgressFunctor(timestamp_t _t) : t(_t) {}
