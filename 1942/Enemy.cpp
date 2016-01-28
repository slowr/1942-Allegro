#include "Enemy.h"

void Enemy::EnemyCollisionHandler::operator()(Sprite *caller, Sprite *arg) {
	if (arg->GetType() == spritetype_t::PLAYER_BULLET){
		std::cout << "enemy hit by player bullet" << std::endl;
		PlayerBullet *b = dynamic_cast<PlayerBullet *>(arg);
		b->StopBullet();
	}
}

Enemy::Enemy(void) : Sprite(SCREEN_W / 2 - AnimationFilmHolder::Get().GetFilm("enemy.blue_plane")->GetFrameBox(0).w / 2, SCREEN_H / 2, AnimationFilmHolder::Get().GetFilm("enemy.blue_plane"), spritetype_t::ENEMY){
	AddCollisionHandler(new EnemyCollisionHandler());
	animation = new MovingAnimation(0, speed, delay, true, 1);
	animator = new MovingAnimator();
	animator->Start(this, animation, 0);
	AnimatorHolder::Register(animator);
	//AnimatorHolder::MarkAsRunning(animator);
	SpriteHolder::Get().Add(this);
}

void Enemy::Draw(ALLEGRO_BITMAP *dest){
	AnimationFilmHolder::Get().GetFilm("enemy.blue_plane")->DisplayFrame(dest, Point(x, y), frameNo);
}