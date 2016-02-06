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

void AnimatorHolder::Progress() {
	std::for_each(
		running.begin(), running.end(), ProgressFunctor()
		);
}

void AnimatorHolder::ProgressFunctor::operator()(Animator* a) const { 
	a->Progress(); 
}

AnimatorHolder::ProgressFunctor::ProgressFunctor() {}
