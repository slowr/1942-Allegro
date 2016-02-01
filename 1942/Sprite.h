#pragma once

#include "types.h"
#include "AnimationFilm.h"
#include <assert.h>
#include <iostream>
#include <list>

class Sprite {
protected:
	float x, y;
	bool isVisible;
	byte frameNo;
	Rect frameBox;
	spritetype_t type;
	spritestate_t state;
	AnimationFilm* currFilm;
	void NotifyCollision(Sprite* arg);
public:
	spritetype_t GetType(void);
	spritestate_t GetState(void);
	void SetState(spritestate_t _s);
	void ClearHandlers(void);
	void CollisionCheck(Sprite* s);
	void SetFrame(byte i);
	byte GetFrame(void) const;
	float GetX(void);
	float GetY(void);
	bool IsVisible(void) const;
	void Move(offset_t dx, offset_t dy);
	Sprite(float _x, float _y, AnimationFilm* film, spritetype_t _type);
	virtual void CollisionResult(spritetype_t type) {};
	virtual void Draw(ALLEGRO_BITMAP * dest);
	virtual void AnimationFinish(void) {};
	virtual ~Sprite();
};