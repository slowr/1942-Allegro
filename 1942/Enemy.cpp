#include "Enemy.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Player.h"

Enemy::Enemy(float _x, float _y, std::string sprite, enemysubtype_t t) : 
Sprite(_x, _y, AnimationFilmHolder::Get().GetFilm(sprite), spritetype_t::ENEMY), subtype(t){
	AnimationInit();
}


void Enemy::AnimationInit(){
	std::list<PathEntry *> p;
	PathEntry * pE;
	switch (subtype){
	case RED:
		pE = new PathEntry();
		pE->dx = 5;
		pE->dy = 0;
		pE->delay = 50;
		pE->frame = 0;
		pE->repetitions = (SCREEN_W - x) / pE->dx;
		p.push_back(pE);
		animation = new MovingPathAnimation(p, 1);
		animator = new MovingPathAnimator();
		animator->Start(this,animation, TIMESTAMP(tickCount));
		animator->SetOnFinish(OnAnimationFinish, this);
		AnimatorHolder::Register(animator);
		AnimatorHolder::MarkAsRunning(animator);
		break;
	default:
		//std::ostringstream oss;
		float px, py;
		float oldDx = x;
		float oldDy = y;
		float radius = 200;
		// oss << radius << std::endl;
		float center_x = x - radius;
		float center_y = y;
		for (float degrees = 0; degrees <= 360; degrees++){
			float radians = degrees*(M_PI / 180);
			px = radius*cos(radians) + center_x;
			py = radius*sin(radians) + center_y;
			PathEntry * pE = new PathEntry();
			float dx_move = px - oldDx;
			float dy_move = py - oldDy;
			pE->dx = dx_move;
			pE->dy = dy_move;
			//oss << dx_move << ":" << dy_move << " ";
			if (degrees >= 0 && degrees < 11.25)
				pE->frame = 0;
			else if (degrees >= 11.25 && degrees < 33.75)
				pE->frame = 15;
			else if (degrees >= 33.75 && degrees < 56.25)
				pE->frame = 14;
			else if (degrees >= 56.25 && degrees < 78.75)
				pE->frame = 13;
			else if (degrees >= 78.75 && degrees < 101.25)
				pE->frame = 12;
			else if (degrees >= 101.25 && degrees < 123.75)
				pE->frame = 11;
			else if (degrees >= 123.75 && degrees < 143.25)
				pE->frame = 10;
			else if (degrees >= 143.25 && degrees < 168.75)
				pE->frame = 9;
			else if (degrees >= 168.75 && degrees < 191.25)
				pE->frame = 8;
			else if (degrees >= 191.25 && degrees <= 213.75)
				pE->frame = 7;
			else if (degrees >= 213.75 && degrees <= 236.25)
				pE->frame = 6;
			else if (degrees >= 236.25 && degrees <= 258.75)
				pE->frame = 5;
			else if (degrees >= 258.75 && degrees <= 281.25)
				pE->frame = 4;
			else if (degrees >= 281.25 && degrees <= 303.75)
				pE->frame = 3;
			else if (degrees >= 303.75 && degrees <= 326.25)
				pE->frame = 2;
			else if (degrees >= 326.25 && degrees <= 348.75)
				pE->frame = 1;
			else if (degrees >= 348.75 && degrees <= 360)
				pE->frame = 0;

			pE->repetitions = 1;
			pE->delay = 10;
			p.push_back(pE);
			oldDx = px;
			oldDy = py;
		}

		//{
		//	std::ofstream outFile;
		//	std::string name = "resources/MovementStepsRadius.data";
		//	oss << std::endl;
		//	outFile.open(name.c_str(), std::ios_base::app);
		//	outFile << oss.str() << std::endl;
		//	outFile.close();
		//}

		animation = new MovingPathAnimation(p, 1);
		animator = new MovingPathAnimator();
		animator->Start(this, animation, TIMESTAMP(tickCount));
		animator->SetOnFinish(OnAnimationFinish, this);
		AnimatorHolder::Register(animator);
		AnimatorHolder::MarkAsRunning(animator);
		break;
	}
}

enemysubtype_t Enemy::GetSubType(){
	return subtype;
}

void Enemy::CollisionResult(Sprite *s){
	switch (s->GetType()){
	case spritetype_t::PLAYER_BULLET:
	case spritetype_t::PLAYER:
		if (s->GetType() == PLAYER){
			if (((Player *)s)->GetMovement() == TUMBLE) return;
		}
		state = spritestate_t::DEAD;
		break;
	}
}

void Enemy::AnimationFinish(void){
	LatelyDestroyable::Add(this);
}

Enemy::~Enemy(){
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}