#include "EnemyBullet.h"
#include "AnimationFilmHolder.h"
#include "MovingPathAnimation.h"
#include "MovingPathAnimator.h"
#include "AnimatorHolder.h"
#include "GameController.h"

float GetLineY(float x1, float y1, float x2, float y2, float x){
	float m = (y2 - y1) / (x2 - x1);
	float b = y1 - (m * x1);

	return m*x + b;
}

float GetLineX(float x1, float y1, float x2, float y2, float y){
	float m = (y2 - y1) / (x2 - x1);
	float b = y1 - (m * x1);

	return (y - b) / m;
}

EnemyBullet::EnemyBullet(float x, float y) : Sprite(x, y, AnimationFilmHolder::Get().GetFilm("enemy.bullet"), spritetype_t::ENEMY_BULLET) {
	std::list<PathEntry *> p;
	PathEntry *pE;

	pE = new PathEntry();

	float dx, dy, distance;
	int repetitions;

	Player *player = GameController::Get().getPlayer();
	Rect playerFrameBox = player->GetFrameBox(player->GetFrame());
	float playerX = player->GetX() + playerFrameBox.w / 2;
	float playerY = player->GetY() + playerFrameBox.h / 2;

	dx = playerX - x;
	dy = playerY - y;

	float tmpX, tmpY;
	if (dx < 0 && dy < 0){
		// top left
		std::cout << "top left\n";
		// top
		if ((tmpX = GetLineX(x, y, playerX, playerY, 0)) >= 0){
			std::cout << "top\n";
			distance = sqrt(pow(tmpX - x, 2) + pow(y, 2));
			tmpY = 0;
		}

		// left
		else if ((tmpY = GetLineY(x, y, playerX, playerY, 0)) >= 0){
			distance = sqrt(pow(x, 2) + pow(y - tmpY, 2));
			tmpX = 0;
		}
	}
	else if (dx < 0 && dy > 0){
		// Bottom left
		std::cout << " Bottom left\n";
		// left
		if ((tmpY = GetLineY(x, y, playerX, playerY, 0)) <= SCREEN_H){
			distance = sqrt(pow(x, 2) + pow(y - tmpY, 2));
			tmpX = 0;
		}

		// bottom
		else if ((tmpX = GetLineX(x, y, playerX, playerY, SCREEN_H)) >= 0){
			distance = sqrt(pow(x - tmpX, 2) + pow(y - SCREEN_H, 2));
			tmpY = SCREEN_H;
		}
	}
	else if (dx > 0 && dy < 0){
		// top right
		std::cout << "top right\n";
		// top
		if ((tmpX = GetLineX(x, y, playerX, playerY, 0)) <= SCREEN_W){
			distance = sqrt(pow(tmpX - x, 2) + pow(y, 2));
			tmpY = 0;
		}

		// right
		else if ((tmpY = GetLineY(x, y, playerX, playerY, SCREEN_W)) >= 0){
			distance = sqrt(pow(x - SCREEN_W, 2) + pow(y - tmpY, 2));
			tmpX = SCREEN_W;
		}
	}
	else {
		// bottom right
		std::cout << "bottom right\n";
		// bottom
		if ((tmpX = GetLineX(x, y, playerX, playerY, SCREEN_H)) >= SCREEN_W){
			distance = sqrt(pow(x - tmpX, 2) + pow(y - SCREEN_H, 2));
			tmpY = SCREEN_H;
		}

		// right
		else if ((tmpY = GetLineY(x, y, playerX, playerY, SCREEN_W)) >= SCREEN_H){
			distance = sqrt(pow(x - SCREEN_W, 2) + pow(y - tmpY, 2));
			tmpX = SCREEN_W;
		}
	}

	dx = tmpX - x;
	dy = tmpY - y;

	pE->delay = 25;
	pE->frame = 0;

	repetitions = distance / speed;
	pE->dx = dx / repetitions;
	pE->dy = dy / repetitions;
	pE->repetitions = repetitions;

	p.push_back(pE);

	animation = new MovingPathAnimation(p, 1);
	animator = new MovingPathAnimator();
	animator->Start(this, animation);
	animator->SetOnFinish(OnAnimationFinish, this);
	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
}

EnemyBullet::~EnemyBullet()
{
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}

void EnemyBullet::CollisionResult(Sprite *s){
	switch (s->GetType()){
	case spritetype_t::PLAYER:
		if (((Player *)s)->GetMovement() == TUMBLE) return;
		state = spritestate_t::DEAD;
		break;
	}
}

void EnemyBullet::AnimationFinish(void){
	LatelyDestroyable::Add(this);
}
