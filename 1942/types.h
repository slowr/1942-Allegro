#pragma once

#include <Windows.h>

#define TIMESTAMP(x) x*(1000/FPS)

typedef unsigned char frame_t;
typedef signed char offset_t;
typedef unsigned short delay_t;
typedef unsigned short animid_t;
typedef unsigned long timestamp_t;

extern unsigned long tickCount;

const float FPS = 25;
const float SCREEN_W = 800;
const float SCREEN_H = 600;
const int BG_SCROLL_SPEED = 25;		// pixels per second

//timestamp_t getSystemTime(void){
//	SYSTEMTIME time;
//	GetSystemTime(&time);
//	return (time.wSecond * 1000) + time.wMilliseconds;
//}

class Point
{
public:
	Point() : x(), y() {}
	Point(float _x, float _y) :
		x(_x), y(_y) {}

	float x;
	float y;
};

class Rect
{
public:
	Rect() : left(), top(), right(), bottom() {}
	Rect(float left, float top, float right, float bottom) :
		left(left), top(top), right(right), bottom(bottom) {}
	Rect(Point p, float width, float height) :
		left(p.x), top(p.y), right(p.x + width), bottom(p.y + height) {
		h = height;
		w = width;
	}

	float h;
	float w;
	float left;
	float top;
	float right;
	float bottom;
};

enum spritetype_t {
	PLAYER, PLAYER_BULLET, ENEMY_BULLET, ENEMY, POWER_UP, UI, EXPLOSION
};

enum spritestate_t {
	ALIVE, DEAD, WAIT
};