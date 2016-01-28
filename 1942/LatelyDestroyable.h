#pragma once

#include <list>
#include <assert.h>
#include <algorithm>

class DestructionManager {
	static std::list<LatelyDestroyable*> dead;
public:
	static void Register(LatelyDestroyable* o);
	static void Commit(void);
};

class LatelyDestroyable {
	friend class DestructionManager;
	bool alive;
	bool inDestruction;
	class Delete : public std::unary_function<LatelyDestroyable*, void>
	{
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
