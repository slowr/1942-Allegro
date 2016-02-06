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
	byte GetTotalFrames(void) const;
	ALLEGRO_BITMAP *GetBitmap(void) const;
	const std::string GetId(void) const;
	const Rect GetFrameBox(byte frameNo) const;
	void DisplayFrame(const Point & at, byte frameNo) const;
	AnimationFilm(ALLEGRO_BITMAP *source, const std::vector<Rect> rects, const std::string& sid);
};