#include "SpriteHolder.h"
#include "SmallEnemyExplosion.h"
#include "PlayerExplosion.h"
#include "Enemy.h"

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
	case spritetype_t::POWER_UP:
		GetSprites(spritetype_t::PLAYER, list);
		for (SpriteList::iterator it = list->begin(); it != list->end(); ++it) {
			CollisionChecker::Get().Register(s, *it);
		}
		break;
	default:
		break;
	}
	delete list;
}

void SpriteHolder::DestroyEnemies(void) {
	for (Sprite *e : sprites[spritetype_t::ENEMY]) {
		if(!GameController::Get().isPlayerDead() )((Enemy *)e)->Explode();
		e->SetState(spritestate_t::DEAD);
	}
}

void SpriteHolder::Remove(Sprite* s){
	sprites[s->GetType()].remove(s);
	CollisionChecker::Get().CancelAll(s);
}

void SpriteHolder::GetSprites(spritetype_t type, SpriteList* result) {
	SpritesByType::iterator i = sprites.find(type);
	if (i != sprites.end())
		*result = i->second;
}

void SpriteHolder::DrawSprites(){
	SpriteList * result = new SpriteList();
	for (int i = 0; i <= spritetype_t::UI; i++){
		
		GetSprites((spritetype_t)i, result);
		for (std::list<Sprite *>::iterator it = result->begin(); it != result->end(); ++it){
			Sprite * s = (*it);
			spritestate_t state = s->GetState();

			if (state == spritestate_t::ALIVE){
				s->Draw();
			}
			else if (state == spritestate_t::DEAD) {
				if (s->GetType() != spritetype_t::PLAYER) LatelyDestroyable::Add(s);
				s->SetState(spritestate_t::WAIT);
			}
		}
	}
	delete result;
}

SpriteHolder& SpriteHolder::Get(void) { 
	return holder; 
}