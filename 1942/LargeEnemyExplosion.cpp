#include "LargeEnemyExplosion.h"
#include "LatelyDestroyable.h"
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_audio.h>

LargeEnemyExplosion::LargeEnemyExplosion(float x, float y, std::string film) : Sprite(x, y, AnimationFilmHolder::Get().GetFilm(film), spritetype_t::EXPLOSION)
{
	ALLEGRO_SAMPLE* explosion = al_load_sample("resources/L_explosion.ogg");
	animation = new FrameRangeAnimation(0, 10, 0, 0, delay, false, 2);
	animator = new FrameRangeAnimator();
	animator->Start(this, animation);
	al_play_sample(explosion, 0.7, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
	animator->SetOnFinish(OnAnimationFinish, this);
	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
}


LargeEnemyExplosion::~LargeEnemyExplosion()
{
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}

void LargeEnemyExplosion::CollisionResult(spritetype_t type) {
	return;
}

void LargeEnemyExplosion::AnimationFinish() {
	LatelyDestroyable::Add(this);
}