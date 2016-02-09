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
	void NotifyCollision(Sprite* arg);
public:
	AnimationFilm* currFilm;
	spritetype_t GetType(void);
	spritestate_t GetState(void);
	void SetState(spritestate_t _s);
	void CollisionCheck(Sprite* s);
	void SetFrame(byte i);
	byte GetFrame(void) const;
	Rect GetFrameBox(byte i) const;
	void SetX(float _x);
	void SetY(float _y);
	float GetX(void);
	float GetY(void);
	bool IsVisible(void) const;
	virtual void Move(offset_t dx, offset_t dy);
	Sprite(float _x, float _y, AnimationFilm* film, spritetype_t _type);
	virtual void CollisionResult(Sprite *s) {};
	virtual void Draw();
	virtual void AnimationFinish(void) {};
	virtual ~Sprite();
};