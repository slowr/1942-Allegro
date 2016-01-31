#pragma once

#include <list>
#include <assert.h>  
#include <algorithm>
#include <iostream>
#include "Sprite.h"
#include "Animator.h"
#include "SpriteHolder.h"
#include "AnimatorHolder.h"

typedef std::list<Sprite *> DeadList;

class LatelyDestroyable {
	static DeadList dead;
public:
	static void Add(Sprite *o);
	static void Destroy(void);
};

void OnAnimationFinish(Animator *a, void *d);
