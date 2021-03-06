#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <map>
#include "Sprite.h"
#include "CollisionChecker.h"

class SpriteHolder {
private:
	typedef std::list<Sprite*> SpriteList;
	static SpriteHolder holder;
protected:
	typedef std::map<unsigned, SpriteList> SpritesByType;
	SpritesByType sprites;
public:
	void Add(Sprite* s);
	void Remove(Sprite* s);
	void GetSprites(spritetype_t type, SpriteList* result);
	void DrawSprites();
	void DestroyEnemies(bool explode);
	void DestroyPows();
	static SpriteHolder& Get(void);
};