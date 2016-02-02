#include "Enemy.h"
#define _USE_MATH_DEFINES
#include <math.h>

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
		pE->dx = 10;
		pE->dy = 0;
		pE->delay = 100;
		pE->frame = 0;
		pE->repetitions = 40;
		p.push_back(pE);
		animation = new MovingPathAnimation(p, 1);
		animator = new MovingPathAnimator();
		animator->Start(this,animation, 0);
		animator->SetOnFinish(OnAnimationFinish, this);
		AnimatorHolder::Register(animator);
		AnimatorHolder::MarkAsRunning(animator);
		break;
	default:
		//std::ostringstream oss;
		float px, py;
		float oldDx = x;
		float oldDy = y;
		float radius = 100;
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
			if (degrees >= 0 && degrees < 22.5)
				pE->frame = 0;
			else if (degrees >= 22.5 && degrees < 67.5)
				pE->frame = 7;
			else if (degrees >= 67.5 && degrees < 112.5)
				pE->frame = 6;
			else if (degrees >= 112.5 && degrees < 157.5)
				pE->frame = 5;
			else if (degrees >= 157.5 && degrees < 202.5)
				pE->frame = 4;
			else if (degrees >= 202.5 && degrees < 247.5)
				pE->frame = 3;
			else if (degrees >= 247.5 && degrees < 292.5)
				pE->frame = 2;
			else if (degrees >= 292.5 && degrees < 337.5)
				pE->frame = 1;
			else if (degrees >= 337.5 && degrees <= 360)
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
		animator->Start(this, animation, 0);
		animator->SetOnFinish(OnAnimationFinish, this);
		AnimatorHolder::Register(animator);
		AnimatorHolder::MarkAsRunning(animator);
		break;
	}
}

enemysubtype_t Enemy::GetSubType(){
	return subtype;
}

void Enemy::CollisionResult(spritetype_t type){
	switch (type){
	case spritetype_t::PLAYER_BULLET:
	case spritetype_t::PLAYER:
		state = spritestate_t::DEAD;
		isVisible = false;
		break;
	}
}

void Enemy::AnimationFinish(void){
	std::cout << x << y << std::endl;
	//LatelyDestroyable::Add(this);
}

Enemy::~Enemy(){
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}