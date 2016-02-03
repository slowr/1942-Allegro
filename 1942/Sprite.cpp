#include "Sprite.h"
#include "SpriteHolder.h"

void Sprite::NotifyCollision(Sprite* arg){
	this->CollisionResult(arg);
	arg->CollisionResult(this);
}

spritetype_t Sprite::GetType(void) {
	return type;
}

spritestate_t Sprite::GetState(void) {
	return state;
}

void Sprite::SetX(float _x){
	x = _x;
}

void Sprite::SetY(float _y){
	y = _y;
}

void Sprite::SetState(spritestate_t _s) {
	state = _s;
}

void Sprite::CollisionCheck(Sprite* s){
	if ( !((this->state == spritestate_t::ALIVE) && (s->state == spritestate_t::ALIVE))) return;
	float x1, y1, x2, y2, x3, y3, x4, y4;
	x1 = x;
	y1 = y;
	x2 = x + frameBox.w * 2;
	y2 = y + frameBox.h * 2;
	x3 = s->x;
	y3 = s->y;
	x4 = s->x + s->frameBox.w * ScaleFactor;
	y4 = s->y + s->frameBox.h * ScaleFactor;
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

Rect Sprite::GetFrameBox(byte i) const{
	assert(i < currFilm->GetTotalFrames());
	return currFilm->GetFrameBox(i);
}

float Sprite::GetX(void){
	return x;
}

float Sprite::GetY(void){
	return y;
}

byte Sprite::GetFrame(void) const { return frameNo; }
bool Sprite::IsVisible(void) const { return isVisible; }

void Sprite::Move(offset_t dx, offset_t dy) {
	x += dx;
	y += dy;
}

Sprite::Sprite(float _x, float _y, AnimationFilm* film, spritetype_t _type) :
x(_x), y(_y), currFilm(film), isVisible(true), type(_type), state(spritestate_t::ALIVE){
	frameNo = currFilm->GetTotalFrames(); SetFrame(0);
	SpriteHolder::Get().Add(this);
}

void Sprite::Draw(ALLEGRO_BITMAP *dest){
	currFilm->DisplayFrame(dest, Point(x, y), frameNo);
}

Sprite::~Sprite(void){
	SpriteHolder::Get().Remove(this);

}