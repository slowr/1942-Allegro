#pragma once

#include <list>
#include <algorithm>
#include "Sprite.h"

class CollisionChecker {
	typedef std::pair<Sprite *, Sprite *> Pair;
	std::list<Pair> pairs;

	struct CheckFunctor : public std::unary_function<Pair, void> {
		void operator()(const Pair& p) const;
	};
	static CollisionChecker collision_checker;
public:
	void Register(Sprite *s1, Sprite *s2);
	void Cancel(Sprite *s1, Sprite *s2);
	void Check(void) const;
	void CancelAll(Sprite *s);
	static CollisionChecker& Get();
};