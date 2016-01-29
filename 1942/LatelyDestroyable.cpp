#include "LatelyDestroyable.h"

DeadList LatelyDestroyable::dead;

void LatelyDestroyable::Add(DestroyableObject *o){
	dead.push_back(o);
}

void LatelyDestroyable::Destroy(void){
	for (DeadList::iterator it = dead.begin(); it != dead.end(); ++it){
		AnimatorHolder::MarkAsSuspended((*it)->animator);
		AnimatorHolder::Cancel((*it)->animator);
		SpriteHolder::Get().Remove((Sprite *) (*it)->sprite);
	}
}