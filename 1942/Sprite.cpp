#include "Sprite.h"

Sprite::CollisionHandler::~CollisionHandler(){}

void Sprite::NotifyCollision(Sprite* arg){
	for (Handlers::iterator i = handlers.begin(); i != handlers.end(); ++i)
		(**i)(this, arg);
}

unsigned Sprite::GetType(void) { 
	return type; 
}

unsigned Sprite::GetState(void) { 
	return state; 
}

void Sprite::SetState(unsigned _s) { 
	state = _s; 
}

void Sprite::AddCollisionHandler(CollisionHandler * h){
	handlers.push_back(h);
}

void Sprite::ClearHandlers(void){
	for (Handlers::iterator i = handlers.begin(); i != handlers.end(); ++i)
		delete *i;
	handlers.clear();
}

void Sprite::CollisionCheck(Sprite* s){
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

void Sprite::SetFrame(byte i) {
	if (i != frameNo) {
		assert(i < currFilm->GetTotalFrames());
		frameBox = currFilm->GetFrameBox(frameNo = i);
	}
}

byte Sprite::GetFrame(void) const { return frameNo; }
bool Sprite::IsVisible(void) const { return isVisible; }

void Sprite::Move(offset_t dx, offset_t dy) {
	x += dx;
	y += dy;
}

Sprite::Sprite(float _x, float _y, AnimationFilm* film, spritetype_t _type) :
	x(_x), y(_y), currFilm(film), isVisible(true), type(_type)
{
	frameNo = currFilm->GetTotalFrames(); SetFrame(0);
}

	Sprite::~Sprite(){ ClearHandlers(); }