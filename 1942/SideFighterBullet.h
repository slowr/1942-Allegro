#pragma once
#include "Sprite.h"
#include "MovingAnimation.h"
#include "MovingAnimator.h"

class SideFighterBullet : public Sprite
{
	MovingAnimator * animator;
	MovingAnimation * animation;
	const static int speed = 10;
	const static int delay = 15;
	void StopBullet();
public:
	SideFighterBullet(float _x, float _y);
	~SideFighterBullet();
	virtual void CollisionResult(Sprite *s);
	virtual void Draw();
};

