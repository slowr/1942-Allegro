#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <map>
#include "Sprite.h"

class SpriteHolder {
private:
	typedef std::list<Sprite*> SpriteList;
	static SpriteHolder holder;
protected:
	typedef std::map<unsigned, SpriteList> SpritesByType;
	SpritesByType sprites;
public:
	void Add(Sprite* s){
		sprites[s->GetType()].push_back(s);
	}
	void Remove(Sprite* s){
		sprites[s->GetType()].remove(s);
	}
	void GetSprites(unsigned type, SpriteList* result) {
		SpritesByType::iterator i = sprites.find(type);
		if (i != sprites.end())
			*result = i->second;
	}
	static SpriteHolder& Get(void) { return holder; }
};