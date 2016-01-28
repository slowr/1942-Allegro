#include "DestructionManager.h"

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