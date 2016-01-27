#ifndef _BITMAPLOADER_H_
#define _BITMAPLOADER_H_

#pragma once

#include <allegro5/allegro.h>
#include <string>
#include <map>

#include <iostream>

class BitmapLoader {
private:
	typedef std::map<std::string, ALLEGRO_BITMAP *> Bitmaps;
	Bitmaps bitmaps;
	ALLEGRO_BITMAP *GetBitmap(const std::string path) const {
		Bitmaps::const_iterator i = bitmaps.find(path); 
		return i != bitmaps.end() ? i->second : (ALLEGRO_BITMAP *)0;
	}
public:
	ALLEGRO_BITMAP *Load(const std::string& path) {
		ALLEGRO_BITMAP *b = GetBitmap(path);
		if (!b) {
			bitmaps[path] = (b = al_load_bitmap(path.c_str()));
			std::cout << path.c_str() << std::endl;
			assert(b);
		}
		return b;
	} 

	// prefer to massively clear bitmaps at the end than 
	// to destroy individual bitmaps during gameplay 
	void CleanUp(void) {
		for (Bitmaps::iterator i = bitmaps.begin(); i != bitmaps.end(); ++i)
			al_destroy_bitmap(i->second);
		bitmaps.clear();
	}

	BitmapLoader(void) {}
	
	~BitmapLoader(){
		CleanUp();
	}
};


#endif