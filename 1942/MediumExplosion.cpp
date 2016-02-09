#include "MediumExplosion.h"
#include "LatelyDestroyable.h"
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_audio.h>


MediumExplosion::MediumExplosion(float x, float y) : Sprite(x, y, AnimationFilmHolder::Get().GetFilm("medium.explosion"), spritetype_t::EXPLOSION)
{
	ALLEGRO_SAMPLE* explosion = al_load_sample("resources/SM_explosion.ogg");
	animation = new FrameRangeAnimation(0, 5, 0, 0, delay, false, 2);
	animator = new FrameRangeAnimator();
	animator->Start(this, animation);
	al_play_sample(explosion, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
	animator->SetOnFinish(OnAnimationFinish, this);
	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
}

void MediumExplosion::CollisionResult(spritetype_t type) {
	return;
}

void MediumExplosion::AnimationFinish() {
	LatelyDestroyable::Add(this);
}


MediumExplosion::~MediumExplosion()
{
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}
