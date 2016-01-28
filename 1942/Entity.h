#pragma once

#include "Sprite.h"
#include "SpriteHolder.h"

class Entity {
protected:
	Sprite *sprite;
public:
	Entity() { }
	void setSprite(Sprite *s){
		sprite = s;
		SpriteHolder::Get().Add(s);
	};
	Sprite *getSprite(){
		return sprite;
	}
};