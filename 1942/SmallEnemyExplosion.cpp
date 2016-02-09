#include "SmallEnemyExplosion.h"
#include "LatelyDestroyable.h"
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_audio.h>

SmallEnemyExplosion::SmallEnemyExplosion(int x, int y) : 
Sprite(x, y, AnimationFilmHolder::Get().GetFilm("enemy.explosion"), spritetype_t::EXPLOSION){

	ALLEGRO_SAMPLE* explosion = al_load_sample("resources/SM_explosion.ogg");
	animation = new FrameRangeAnimation(0, 5, 0, 0, delay, false, 2);
	animator = new FrameRangeAnimator();
	animator->Start(this, animation);
	al_play_sample(explosion, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
	animator->SetOnFinish(OnAnimationFinish, this);
	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
}

void SmallEnemyExplosion::CollisionResult(spritetype_t type){
	return;
}

SmallEnemyExplosion::~SmallEnemyExplosion(){
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}

void SmallEnemyExplosion::AnimationFinish(){
	LatelyDestroyable::Add(this);
}

