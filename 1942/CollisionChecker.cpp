#include "CollisionChecker.h"

CollisionChecker CollisionChecker::collision_checker;

void CollisionChecker::CheckFunctor::operator()(const Pair& p) const {
	p.first->CollisionCheck(p.second);
}

void CollisionChecker::Register(Sprite *s1, Sprite *s2){
	pairs.push_back(Pair(s1, s2));
}

void CollisionChecker::Cancel(Sprite *s1, Sprite *s2){
	pairs.remove(Pair(s1, s2));
}

void CollisionChecker::Check(void) const {
	std::for_each(
		pairs.begin(), pairs.end(), CheckFunctor()
		);
}

CollisionChecker& CollisionChecker::Get(){
	return collision_checker; 
}

