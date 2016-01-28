#pragma once

#include <list>
#include <algorithm>
#include "Sprite.h"

class CollisionChecker {
	typedef std::pair<Sprite *, Sprite *> Pair;
	std::list<Pair> pairs;

	struct CheckFunctor : public std::unary_function<Pair, void> {
		void operator()(const Pair& p) const { 
			p.first->CollisionCheck(p.second); 
		}
	};

	static CollisionChecker collision_checker;
public:
	void Register(Sprite *s1, Sprite *s2){
		pairs.push_back(Pair(s1, s2));
	}
	void Cancel(Sprite *s1, Sprite *s2){
		pairs.remove(Pair(s1, s2));
	}
	void Check(void) const {
		std::cout << "CHECK!" << std::endl;
		std::for_each(
			pairs.begin(), pairs.end(), CheckFunctor()
			);
	}

	static CollisionChecker& Get(){ return collision_checker; }
};