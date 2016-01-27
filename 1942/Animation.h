#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#pragma once

#include "types.h"

class Animation {
	animid_t id;
public:
	animid_t GetId(void) { return id; }
	virtual Animation* Clone(animid_t newId) const = 0;
	Animation(animid_t _id) : id(_id) {}
	virtual ~Animation() {}
};

#endif