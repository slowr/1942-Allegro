#pragma once

#include "types.h"
#include "AnimationFilm.h"
#include <assert.h>
#include <iostream>
#include <list>

class Sprite {
public:
	class CollisionHandler {
	public:
		virtual void operator()(Sprite *caller, Sprite *arg) = 0;
		virtual ~CollisionHandler();
	};
protected:
	float x, y;
	bool isVisible;
	byte frameNo;
	Rect frameBox;
	AnimationFilm* currFilm;
	typedef std::list<CollisionHandler*> Handlers;
	spritetype_t type;
	unsigned state;
	Handlers handlers;
	void NotifyCollision(Sprite* arg);
public:
	unsigned GetType(void);
	unsigned GetState(void);
	void SetState(unsigned _s);
	void AddCollisionHandler(CollisionHandler * h);
	void ClearHandlers(void);
	void CollisionCheck(Sprite* s);
	void SetFrame(byte i);
	byte GetFrame(void) const;
	bool IsVisible(void) const;
	void Move(offset_t dx, offset_t dy);
	Sprite(float _x, float _y, AnimationFilm* film, spritetype_t _type);
	virtual ~Sprite();
};