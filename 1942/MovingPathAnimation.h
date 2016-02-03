#pragma once

#include "Animation.h"
#include <list>

enum pathentryaction_t {
	NO_ACTION, SHOOT
};

struct PathEntry {
	offset_t dx, dy;
	frame_t frame;
	delay_t delay;
	int repetitions;
	pathentryaction_t action;
	PathEntry(void);
	PathEntry(const PathEntry& p);
};

class MovingPathAnimation : public Animation {
	std::list<PathEntry *> path;
public:
	std::list<PathEntry *>& GetPath(void);
	void SetPath(const std::list<PathEntry *>& p);
	Animation* Clone(animid_t newId) const;
	MovingPathAnimation(const std::list<PathEntry *>& _path, animid_t id);
};
