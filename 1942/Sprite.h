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
	void NotifyCollision(Sprite* arg){
		for (Handlers::iterator i = handlers.begin(); i != handlers.end(); ++i)
			(**i)(this, arg);
	}
public:
	unsigned GetType(void) { return type; }
	unsigned GetState(void) { return state; }
	void SetState(unsigned _s) { state = _s; }
	void AddCollisionHandler(CollisionHandler * h){
		handlers.push_back(h);
	}
	void ClearHandlers(void){
		for (Handlers::iterator i = handlers.begin(); i != handlers.end(); ++i)
			delete *i;
		handlers.clear();
	}
	void CollisionCheck(Sprite* s){
		if (!isVisible || !s->isVisible) return;
		float x1, y1, x2, y2, x3, y3, x4, y4;
		x1 = x;
		y1 = y;
		x2 = x + frameBox.w;
		y2 = y + frameBox.h;
		x3 = s->x;
		y3 = s->y;
		x4 = s->x + s->frameBox.w;
		y4 = s->y + s->frameBox.h;
		if (!(x4 < x1 || x2 < x3 || y4 < y1 || y2 < y3)){
			NotifyCollision(s);
		}
	}
	void SetFrame(byte i) {
		if (i != frameNo) {
			assert(i < currFilm->GetTotalFrames());
			frameBox = currFilm->GetFrameBox(frameNo = i);
		}
	}

	byte GetFrame(void) const { return frameNo; }
	bool IsVisible(void) const { return isVisible; }

	void Move(offset_t dx, offset_t dy) {
		x += dx;
		y += dy;
	}

	Sprite(float _x, float _y, AnimationFilm* film, spritetype_t _type) :
		x(_x), y(_y), currFilm(film), isVisible(true), type(_type)
	{
		frameNo = currFilm->GetTotalFrames(); SetFrame(0);
	}

	~Sprite(){ ClearHandlers(); }
};