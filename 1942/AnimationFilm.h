#ifndef _ANIMATIONFILM_H_
#define _ANIMATIONFILM_H_

#pragma once

#include <vector>
#include <assert.h>
#include <string>
#include "types.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class AnimationFilm {
	std::vector<Rect> boxes;
	ALLEGRO_BITMAP *bitmap;
	std::string id;
public:
	float scaledFactor = 1;
	byte GetTotalFrames(void) const { return boxes.size(); }
	ALLEGRO_BITMAP *GetBitmap(void) const { return bitmap; }
	const std::string GetId(void) const { return id; }

	const Rect GetFrameBox(byte frameNo) const
	{
		assert(boxes.size()>frameNo); 
		return boxes[frameNo];
	}

	void DisplayFrame(ALLEGRO_BITMAP *dest, const Point& at, byte frameNo) const {
		Rect box = boxes.at(frameNo);
		ALLEGRO_BITMAP* tmp = al_create_sub_bitmap(bitmap,
			box.left, box.top, box.w, box.h);
		al_draw_scaled_bitmap(tmp,
			0, 0, box.w, box.h,
			at.x, at.y, box.w * scaledFactor, box.h * scaledFactor, 0);
	}

	AnimationFilm(ALLEGRO_BITMAP *source, const std::vector<Rect> rects, const std::string& sid) : bitmap(source), boxes(rects), id(sid) {}
};


#endif