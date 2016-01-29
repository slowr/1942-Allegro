#pragma once

#include <list>
#include <assert.h>  
#include <algorithm>
#include <iostream>
#include "Sprite.h"
#include "Animator.h"
#include "SpriteHolder.h"
#include "AnimatorHolder.h"

class DestroyableObject {
public:
	Sprite *sprite;
	Animator *animator;
	DestroyableObject(Animator *a, Sprite *s) : sprite(s), animator(a) {};
};

typedef std::list<DestroyableObject *> DeadList;

class LatelyDestroyable {
	static DeadList dead;
public:
	static void Add(DestroyableObject *o);
	static void Destroy(void);
};

void OnAnimationFinish(Animator *a, void *d);
