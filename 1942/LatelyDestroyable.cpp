#include "LatelyDestroyable.h"

DeadList LatelyDestroyable::dead;

void LatelyDestroyable::Add(DestroyableObject *o){
	dead.push_back(o);
}

void LatelyDestroyable::Destroy(void){
	for (DeadList::iterator it = dead.begin(); it != dead.end(); ++it){
		delete (*it)->sprite;
	}
	if(!dead.empty()) dead.clear();
}

void OnAnimationFinish(Animator *a, void *d){
	LatelyDestroyable::Add(new DestroyableObject(a, (Sprite *)d));
}
