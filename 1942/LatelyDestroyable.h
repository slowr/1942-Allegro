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
typedef std::list<Animator *> toBeSuspendedAnimatorsList;
class LatelyDestroyable {
	static int RedPlanesDestroyed;
	static DeadList dead;
	static toBeSuspendedAnimatorsList toBeSuspendedAnimators;
public:
	static void Add(Animator *a);
	static void Add(Sprite *o);
	static void Destroy(void);
};

void OnAnimationFinish(Animator *a, void *d);
