#include "LatelyDestroyable.h"

DeadList LatelyDestroyable::dead;

void LatelyDestroyable::Add(Sprite *o){
	dead.push_back(o);
}

void LatelyDestroyable::Destroy(void){
	for (DeadList::iterator it = dead.begin(); it != dead.end(); ++it){
		delete (*it);
	}
	if(!dead.empty()) dead.clear();
}

void OnAnimationFinish(Animator *a, void *d){
	//LatelyDestroyable::Add((Sprite *)d);
	((Sprite *)d)->AnimationFinish();
}
