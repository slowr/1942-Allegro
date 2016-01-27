#ifndef _LATELYDESTROYABLE_H_
#define _LATELYDESTROYABLE_H_

#pragma once

#include <list>
#include <assert.h>
#include <algorithm>

class DestructionManager {
	static std::list<LatelyDestroyable*> dead;
public:
	static void Register(LatelyDestroyable* o) {
		assert(!o->IsAlive());
		dead.push_back(o);
	}
	static void Commit(void) {
		std::for_each(
			dead.begin(),
			dead.end(),
			LatelyDestroyable::Delete()
			);
		dead.clear();
	}
};

class LatelyDestroyable {
	friend class DestructionManager;
	bool alive;
	bool inDestruction;
	class Delete : public std::unary_function<LatelyDestroyable*, void>
	{
	public: 
		void operator()(LatelyDestroyable* o) const
		{
			o->inDestruction = true; 
			delete o;
		}
	};
	friend class Delete;
public:
	bool IsAlive(void) const { return alive; }
	void Destroy(void) {
		if (alive) {
			alive = false;
			DestructionManager::Register(this);
		}
	}
	LatelyDestroyable(void) : alive(true), inDestruction(false) {}
	virtual ~LatelyDestroyable() { assert(inDestruction); }
};

#endif