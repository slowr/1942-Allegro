#pragma once
#include "Sprite.h"
#include "SpriteHolder.h"
#include "AnimationFilmHolder.h"
#include "types.h"

class PowerUp : public Sprite
{
private:
	powertype_t power;
public:
	void CollisionResult(Sprite *s);
	PowerUp(float _dx, float _dy, powertype_t _power);
	~PowerUp();
};
