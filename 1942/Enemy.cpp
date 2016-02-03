#include "Enemy.h"
#include "PowWave.h"
#include "EnemyBullet.h"

Enemy::Enemy(float _x, float _y, std::string sprite, enemysubtype_t t) : 
Sprite(_x, _y, AnimationFilmHolder::Get().GetFilm(sprite), spritetype_t::ENEMY), subtype(t), health(1){
	AnimationInit();
}


void Enemy::AnimationInit(){
	std::list<PathEntry *> p;
	PathEntry * pE;
	float distance;
	switch (subtype){
	case enemysubtype_t::RED:
		pE = new PathEntry();
		pE->dx = speed;
		pE->dy = 0;
		pE->delay = delay;
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
	case enemysubtype_t::GRAY_MONO:
		float dx, dy;
		int repetitions;
		pE = new PathEntry();

		dx = GameController::Get().getPlayer()->GetX() - x;
		dy = GameController::Get().getPlayer()->GetY() - y;
		distance = sqrt(pow(dx, 2) + pow(dy, 2));
		pE->delay = 25;
		pE->frame = 0;

		repetitions = distance / speed;
		pE->dx = dx / repetitions;
		pE->dy = dy / repetitions;
		pE->repetitions = repetitions - 10;

		p.push_back(pE);

		for (int i = 0; i < 4; i++){
			pE = new PathEntry();

			if (i == 0){
				pE->action = SHOOT;
			}

			pE->delay = 25;
			pE->frame = i + 1;
			pE->repetitions = 5;
			pE->dx = dx / repetitions;
			switch (i) {
			case 0:
			case 1:
				pE->dy = dy / repetitions;
				break;
			case 2:
			case 3:
				pE->dy = -(dy / repetitions);
				break;
			}
			p.push_back(pE);
		}

		pE = new PathEntry();

		pE->delay = 25;
		pE->frame = 5;
		
		pE->dx = dx / repetitions;
		pE->dy = - (dy / repetitions);
		pE->repetitions = repetitions - 10;

		p.push_back(pE);

		animation = new MovingPathAnimation(p, 1);
		animator = new MovingPathAnimator();
		animator->Start(this, animation, TIMESTAMP(tickCount));
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
			pE->delay = 5;
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

/*
	Small green mono : 30, grey = 50
	Small duo : 70, grey = 90
	Small jet : 110, grey = 130
	Medium green : 1000, 100 per hit, grey = 1500, 150 per hit
	Large green : 2000, 100 per hit, grey = 2500, 150 per hit
	Boss : 20000 points
*/
void Enemy::CollisionResult(Sprite *s){
	switch (s->GetType()){
	case spritetype_t::PLAYER_BULLET:
	case spritetype_t::PLAYER:
		if (s->GetType() == PLAYER){
			if (((Player *)s)->GetMovement() == TUMBLE) return;
		}
		if (--health == 0){
			state = spritestate_t::DEAD;
		}
		break;
	}
}

void Enemy::AnimationFinish(void){
	LatelyDestroyable::Add(this);
}

void Enemy::shoot(){
	new EnemyBullet(GetX() + frameBox.w / 2, GetY() + frameBox.h / 2);
}

Enemy::~Enemy(){
	if (subtype == enemysubtype_t::RED) {
		PowWave::Get().OnRedPlaneDead(this);
	}
	AnimatorHolder::MarkAsSuspended(animator);
	AnimatorHolder::Cancel(animator);
	delete animator;
	delete animation;
}