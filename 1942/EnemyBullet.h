#pragma once
#include "Sprite.h"
#include "Animator.h"

class EnemyBullet :
	public Sprite
{
	Animator * animator;
	Animation * animation;
	int health;
	const static int speed = 5;
	const static int delay = 50;
public:
	void CollisionResult(Sprite *s);
	void AnimationFinish(void);
	EnemyBullet(float x, float y);
	virtual ~EnemyBullet();
};

