#include "SpriteHolder.h"
#include "SmallEnemyExplosion.h"
#include "PlayerExplosion.h"
#include "LatelyDestroyable.h"

SpriteHolder SpriteHolder::holder;

void SpriteHolder::Add(Sprite* s){
	sprites[s->GetType()].push_back(s);

	SpriteList * list = new SpriteList();
	switch (s->GetType()){
	case spritetype_t::PLAYER:
		GetSprites(spritetype_t::ENEMY_BULLET, list);
		for (SpriteList::iterator it = list->begin(); it != list->end(); ++it){
			CollisionChecker::Get().Register(s, *it);
		}
		GetSprites(spritetype_t::ENEMY, list);
		for (SpriteList::iterator it = list->begin(); it != list->end(); ++it){
			CollisionChecker::Get().Register(s, *it);
		}
		break;
	case spritetype_t::PLAYER_BULLET:
		GetSprites(spritetype_t::ENEMY, list);
		for (SpriteList::iterator it = list->begin(); it != list->end(); ++it){
			CollisionChecker::Get().Register(s, *it);
		}
		break;
	case spritetype_t::ENEMY:
		GetSprites(spritetype_t::PLAYER, list);
		for (SpriteList::iterator it = list->begin(); it != list->end(); ++it){
			CollisionChecker::Get().Register(s, *it);
		}
		GetSprites(spritetype_t::PLAYER_BULLET, list);
		for (SpriteList::iterator it = list->begin(); it != list->end(); ++it){
			CollisionChecker::Get().Register(s, *it);
		}
		break;
	case spritetype_t::ENEMY_BULLET:
		GetSprites(spritetype_t::PLAYER, list);
		for (SpriteList::iterator it = list->begin(); it != list->end(); ++it){
			CollisionChecker::Get().Register(s, *it);
		}
		break;
	}
}

void SpriteHolder::Remove(Sprite* s){
	sprites[s->GetType()].remove(s);
}

void SpriteHolder::GetSprites(spritetype_t type, SpriteList* result) {
	SpritesByType::iterator i = sprites.find(type);
	if (i != sprites.end())
		*result = i->second;
}

void SpriteHolder::DrawSprites(ALLEGRO_BITMAP *dest){
	for (int i = 0; i <= spritetype_t::EXPLOSION; i++){
		SpriteList * result = new SpriteList();
		GetSprites((spritetype_t)i, result);
		for (std::list<Sprite *>::iterator it = result->begin(); it != result->end(); ++it){
			Sprite * s = (*it);
			spritestate_t state = s->GetState();

			if (state == spritestate_t::ALIVE){
				s->Draw(dest);
			}
			else if (state == spritestate_t::DEAD) {
				if(s->GetType() == spritetype_t::ENEMY)	SmallEnemyExplosion * see = new SmallEnemyExplosion(s->GetX(), s->GetY());
				else if (s->GetType() == spritetype_t::PLAYER) PlayerExplosion * see = new PlayerExplosion(s->GetX(), s->GetY());

				LatelyDestroyable::Add(*it);
			}
		}
	}
}

SpriteHolder& SpriteHolder::Get(void) { 
	return holder; 
}