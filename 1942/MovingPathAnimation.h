#pragma once

#include "Animation.h"
#include <list>

struct PathEntry {
	offset_t dx, dy;
	frame_t frame;
	delay_t delay;
	PathEntry(void);
	PathEntry(const PathEntry& p);
};

class MovingPathAnimation : public Animation {
	std::list<PathEntry> path;
public:
	const std::list<PathEntry>& GetPath(void) const;
	void SetPath(const std::list<PathEntry>& p);
	Animation* Clone(animid_t newId) const;
	MovingPathAnimation(const std::list<PathEntry>& _path, animid_t id);
};
