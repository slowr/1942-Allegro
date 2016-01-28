#pragma once

#include <list>
#include <assert.h>
#include <algorithm>
#include "LatelyDestroyable.h"

class DestructionManager {
	static std::list<LatelyDestroyable*> dead;
public:
	static void Register(LatelyDestroyable* o);
	static void Commit(void);
};