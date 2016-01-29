#include "AnimatorHolder.h"

AnimatorList AnimatorHolder::running, AnimatorHolder::suspended, AnimatorHolder::dead;

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

 void AnimatorHolder::MarkAsDead(Animator* a){
	dead.push_back(a);
 }

void AnimatorHolder::Progress(timestamp_t currTime) {
	for (AnimatorList::iterator it = dead.begin(); it != dead.end(); ++it){
		MarkAsSuspended(*it);
		Cancel(*it);
	}
	dead.clear();
	std::for_each(
		running.begin(), running.end(), ProgressFunctor(currTime)
		);
}

void AnimatorHolder::ProgressFunctor::operator()(Animator* a) const { 
	a->Progress(t); 
}

AnimatorHolder::ProgressFunctor::ProgressFunctor(timestamp_t _t) : t(_t) {}
