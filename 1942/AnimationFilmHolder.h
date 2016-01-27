#ifndef _ANIMATIONFILMHOLDER_H_
#define _ANIMATIONFILMHOLDER_H_

#pragma once

#include <map>
#include <fstream>
#include "AnimationFilm.h"
#include "BitmapLoader.h"
#include <sstream>
#include <bitset>

#define MASKDEPTH 32
typedef uint32_t CollisionMask;

class AnimationFilmHolder {
private:
	typedef std::map<std::string, AnimationFilm*> Films;
	Films films;
	BitmapLoader bitmaps; // only for loading of film bitmaps 
	static AnimationFilmHolder holder; // singleton 
	AnimationFilmHolder(void) {}
	~AnimationFilmHolder() { CleanUp(); }
	static const int MAX_CHARS_PER_LINE = 128;
	const char* const DELIMITER = " ";
public:
	static AnimationFilmHolder& AnimationFilmHolder::Get(void) { return holder; }
	void Load(const std::string& catalogue);
	void CleanUp(void);
	AnimationFilm* GetFilm(const std::string id) const;
};

#endif