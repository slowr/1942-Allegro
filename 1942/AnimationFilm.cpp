#include "AnimationFilm.h"

byte AnimationFilm::GetTotalFrames(void) const { return boxes.size(); }
ALLEGRO_BITMAP * AnimationFilm::GetBitmap(void) const { return bitmap; }
const std::string AnimationFilm::GetId(void) const { return id; }

const Rect AnimationFilm::GetFrameBox(byte frameNo) const
{
	assert(boxes.size()>frameNo);
	return boxes[frameNo];
}

void AnimationFilm::DisplayFrame(const Point& at, byte frameNo) const {
	Rect box = boxes.at(frameNo);
	ALLEGRO_BITMAP* tmp = al_create_sub_bitmap(bitmap,
		box.left, box.top, box.w, box.h);
	al_draw_scaled_bitmap(tmp,
		0, 0, box.w, box.h,
		at.x, at.y, box.w * ScaleFactor, box.h * ScaleFactor, 0);
	al_destroy_bitmap(tmp);
}

AnimationFilm::AnimationFilm(ALLEGRO_BITMAP *source, const std::vector<Rect> rects, const std::string& sid) : bitmap(source), boxes(rects), id(sid) {}
