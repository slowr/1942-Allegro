#include "LatelyDestroyable.h"

void LatelyDestroyable::Delete::operator()(LatelyDestroyable* o) const{
	o->inDestruction = true;
	delete o;
}

bool LatelyDestroyable::IsAlive(void) const { return alive; }
void LatelyDestroyable::Destroy(void) {
	if (alive) {
		alive = false;
		DestructionManager::Register(this);
	}
}
LatelyDestroyable::LatelyDestroyable(void) : alive(true), inDestruction(false) {}
LatelyDestroyable::~LatelyDestroyable() { assert(inDestruction); }
