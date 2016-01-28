#include "MovingPathAnimation.h"

PathEntry::PathEntry(void) : dx(0), dy(0), frame(0xff), delay(0) {}

PathEntry::PathEntry(const PathEntry& p) :
dx(p.dx), dy(p.dy), frame(p.frame), delay(p.delay) {}

const std::list<PathEntry>& MovingPathAnimation::GetPath(void) const{
	return path;
}

void MovingPathAnimation::SetPath(const std::list<PathEntry>& p){
	path.clear(); path = p;
}

Animation* MovingPathAnimation::Clone(animid_t newId) const{
	return new MovingPathAnimation(path, newId);
}

MovingPathAnimation::MovingPathAnimation(const std::list<PathEntry>& _path, animid_t id) :
path(_path), Animation(id) {}