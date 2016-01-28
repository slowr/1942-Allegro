#include "LatelyDestroyable.h"

void DestructionManager::Register(LatelyDestroyable* o) {
	assert(!o->IsAlive());
	dead.push_back(o);
}

void DestructionManager::Commit(void) {
	std::for_each(
		dead.begin(),
		dead.end(),
		LatelyDestroyable::Delete()
		);
	dead.clear();
}


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