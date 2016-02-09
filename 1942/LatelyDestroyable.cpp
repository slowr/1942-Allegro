#include "LatelyDestroyable.h"
#include "Enemy.h"
#include "PowerUp.h"

DeadList LatelyDestroyable::dead;
toBeSuspendedAnimatorsList LatelyDestroyable::toBeSuspendedAnimators;
int LatelyDestroyable::RedPlanesDestroyed;

void LatelyDestroyable::Add(Sprite *o){
	dead.insert(o);
}

void LatelyDestroyable::Add(Animator *a){
	toBeSuspendedAnimators.insert(a);
}

void LatelyDestroyable::Destroy(void){
	for (DeadList::iterator it = dead.begin(); it != dead.end(); ++it){
		Sprite *s = (*it);
		delete (*it);
	}

	for (toBeSuspendedAnimatorsList::iterator it = toBeSuspendedAnimators.begin(); it != toBeSuspendedAnimators.end(); ++it){
		AnimatorHolder::MarkAsSuspended(*it);
	}

	if (!toBeSuspendedAnimators.empty()) toBeSuspendedAnimators.clear();

	if(!dead.empty()) dead.clear();
}

void OnAnimationFinish(Animator *a, void *d){
	//LatelyDestroyable::Add((Sprite *)d);
	((Sprite *)d)->AnimationFinish();
}
