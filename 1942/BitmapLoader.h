#pragma once

#include <allegro5/allegro.h>
#include <string>
#include <map>
#include <iostream>

class BitmapLoader {
private:
	typedef std::map<std::string, ALLEGRO_BITMAP *> Bitmaps;
	Bitmaps bitmaps;
	ALLEGRO_BITMAP *GetBitmap(const std::string path) const;
public:
	ALLEGRO_BITMAP *Load(const std::string& path);
	void CleanUp(void);
	BitmapLoader(void);
	~BitmapLoader();
};
