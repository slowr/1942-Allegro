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
	void Add(Sprite* s){
		sprites[s->GetType()].push_back(s);

		SpriteList * list = new SpriteList();
		switch (s->GetType()){
		case spritetype_t::PLAYER:
			GetSprites(spritetype_t::PLAYER_BULLET, list);
			for (SpriteList::iterator it = list->begin(); it != list->end(); ++it){
				CollisionChecker::Get().Register(s, *it);
			}
			break;
		case spritetype_t::PLAYER_BULLET:
			GetSprites(spritetype_t::PLAYER, list);
			for (SpriteList::iterator it = list->begin(); it != list->end(); ++it){
				CollisionChecker::Get().Register(s, *it);
			}
			break;
		}
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