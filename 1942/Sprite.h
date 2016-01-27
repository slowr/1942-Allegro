#pragma once

#include "types.h"
#include "AnimationFilm.h"
#include <assert.h>
#include <list>

//struct CollisionHandler {
//	virtual void operator()(Sprite *caller, Sprite *arg) const = 0;
//	virtual CollisionHandler* Clone(void) const = 0;
//};

class Sprite {
	byte frameNo;
	Rect frameBox;
	float x, y;
	bool isVisible;
	AnimationFilm* currFilm;
	//protected:
	//	typedef std::list<CollisionHandler*> Handlers;
	//	unsigned type;
	//	unsigned state;
	//	Handlers handlers;
	//	void NotifyCollision(Sprite* arg){
	//		for (Handlers::iterator i = handlers.begin(); i != handlers.end(); ++i)
	//			(**i)(this, arg);
	//	}
public:
	//	unsigned GetType(void) { return type; }
	//	unsigned GetState(void) { return state; }
	//	void SetState(unsigned _s) { state = _s; }
	//	void AddCollisionHandler(const CollisionHandler& h){
	//		handlers.push_back(h.Clone());
	//	}
	//	void ClearHandlers(void){
	//		for (Handlers::iterator i = handlers.begin(); i != handlers.end(); ++i)
	//			delete *i;
	//		handlers.clear();
	//	}
	//	void CollisionCheck(Sprite* s){
	//		//if (s->frameBox overlaps with this->frameBox)
	//			NotifyCollision(s);
	//	}
	void SetFrame(byte i) {
		if (i != frameNo) {
			assert(i < currFilm->GetTotalFrames());
			frameBox = currFilm->GetFrameBox(frameNo = i);
		}
	}

	byte GetFrame(void) const { return frameNo; }
	Rect GetFrameBox(void) const { return frameBox; }
	void SetVisibility(bool v) { isVisible = v; }
	bool IsVisible(void) const { return isVisible; }

	float getX(){ return x; }
	float getY(){ return y; }
	void setX(float _x){ x = _x; }
	void setY(float _y){ y = _y; }

	void Move(offset_t dx, offset_t dy) {
		x += dx;
		y += dy;
	}

	Sprite(float _x, float _y, AnimationFilm* film) :
		x(_x), y(_y), currFilm(film), isVisible(true)
	{
		frameNo = currFilm->GetTotalFrames(); SetFrame(0);
	}
//~Sprite(){ ClearHandlers(); }
};
