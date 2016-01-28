#pragma once

#include <list>
#include <assert.h>
#include <algorithm>
#include "DestructionManager.h"

class LatelyDestroyable {
	friend class DestructionManager;
	bool alive;
	bool inDestruction;
	class Delete : public std::unary_function<LatelyDestroyable*, void>{
	public:
		void operator()(LatelyDestroyable* o) const;
	};
	friend class Delete;
public:
	bool IsAlive(void) const;
	void Destroy(void);
	LatelyDestroyable(void);
	virtual ~LatelyDestroyable();
};

