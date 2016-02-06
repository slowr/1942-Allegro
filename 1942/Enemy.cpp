#include "Enemy.h"
#include "PowWave.h"
#include "EnemyBullet.h"
#include "SmallEnemyExplosion.h"

/*
GREEN_MONO,
GREEN_DOUBLE,
GREEN_JET,
GRAY_MONO,
GRAY_DOUBLE,
GRAY_JET,
GREEN_MID,
GRAY_MID,
GREEN_LARGE,
GRAY_LARGE,
BOSS,
RED
*/

const int HEALTH_PER_TYPE[12] = {
	1,
	1,
	1,
	1,
	1,
	1,
	3,
	5,
	6,
	8,
	15,
	1
};

/*
Small green mono : 30, grey = 50
Small duo : 70, grey = 90
Small jet : 110, grey = 130
Medium green : 1000, 100 per hit, grey = 1500, 150 per hit
Large green : 2000, 100 per hit, grey = 2500, 150 per hit
Boss : 20000 points
*/

const int POINTS_PER_HIT[12] = {
	0,
	0,
	0,
	0,
	0,
	0,
	100,
	150,
	100,
	150,
	0,
	0
};

const int POINTS_PER_DESTRUCTION[12] = {
	30,
	70,
	110,
	50,
	90,
	130,
	1000,
	1500,
	2000,
	2500,
	20000,
	10
};



Enemy::Enemy(float _x, float _y, std::string sprite, enemysubtype_t t) : 
Sprite(_x, _y, AnimationFilmHolder::Get().GetFilm(sprite), spritetype_t::ENEMY), subtype(t), health(HEALTH_PER_TYPE[subtype]){
	AnimationInit();
}

void Enemy::doCircle(Enemy::enemylook_t direction, std::list<PathEntry *> &p, float radius, int resolution = 1) {
	//std::ostringstream oss;
	float px, py;
	float oldDx = 0;
	float oldDy = 0;
	// oss << radius << std::endl;
	oldDx = radius*cos(direction*(M_PI / 180)) - radius;
	oldDy = radius*sin(direction*(M_PI / 180));
	bool doBreakWhenDone = false;
	delay_t delay = (speed * 360) / (360 / resolution);
	PathEntry *pE;
	for (float degrees = direction;; degrees += resolution) {
		if (doBreakWhenDone && degrees >= direction - 1){
			break;
		}
		float radians = degrees*(M_PI / 180);
		px = radius*cos(radians) - radius;
		py = radius*sin(radians);
		pE = new PathEntry();
		float dx_move = px - oldDx;
		float dy_move = py - oldDy;
		pE->dx = dx_move;
		pE->dy = dy_move;
		pE->repetitions = 1;
		pE->delay = delay;
		pE->frame = getFrame(degrees);
		p.push_back(pE);
		oldDx = px;
		oldDy = py;
		if (degrees >= 360) {
			degrees = 0;
			doBreakWhenDone = true;
		}
	}
}

int Enemy::getFrame(float degrees) {
	if (degrees >= 348.75)
		return 0;
	if (degrees >= 326.25)
		return 1;
	if (degrees >= 303.75)
		return 2;
	if (degrees >= 281.25)
		return 3;
	if (degrees >= 258.75)
		return 4;
	if (degrees >= 236.25)
		return 5;
	if (degrees >= 213.75)
		return 6;
	if (degrees >= 191.25)
		return 7;
	if (degrees >= 168.75)
		return 8;
	if (degrees >= 143.25)
		return 9;
	if (degrees >= 123.75)
		return 10;
	if (degrees >= 101.25)
		return 11;
	if (degrees >= 78.75)
		return 12;
	if (degrees >= 56.25)
		return 13;
	if (degrees >= 33.75)
		return 14;
	if (degrees >= 11.25)
		return 15;
	return 0;
}

void Enemy::AnimationInit(){
	std::list<PathEntry *> p;
	PathEntry * pE;
	float distance;
	float loop_start;

	switch (subtype){
	case enemysubtype_t::RED:
		loop_start = ((100*2) / speed) + (rand() % (int) (( (SCREEN_W / 2) - x) / speed));
		pE = new PathEntry();
		pE->dx = speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrame(RIGHT);
		pE->repetitions = loop_start;
		p.push_back(pE);

		doCircle(RIGHT, p, 80, 6);
		
		pE = new PathEntry();
		pE->dx = speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrame(RIGHT);
		pE->repetitions = ((SCREEN_W - x) / speed) - loop_start + 1;
		p.push_back(pE);

		animation = new MovingPathAnimation(p, 1);
		animator = new MovingPathAnimator();
		animator->Start(this,animation);
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
		animator->Start(this, animation);
		animator->SetOnFinish(OnAnimationFinish, this);
		AnimatorHolder::Register(animator);
		AnimatorHolder::MarkAsRunning(animator);
		break;
	default:
		doCircle(BOTTOM, p, 100);

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
		animator->Start(this, animation);
		animator->SetOnFinish(OnAnimationFinish, this);
		AnimatorHolder::Register(animator);
		AnimatorHolder::MarkAsRunning(animator);
		break;
	}
}

enemysubtype_t Enemy::GetSubType(){
	return subtype;
}

void Enemy::Explode(void) {
	new SmallEnemyExplosion(x, y);
}

void Enemy::OnPlaneShot(void) {
	if (--health == 0) {
		if (subtype == enemysubtype_t::RED) {
			PowWave::Get().OnRedPlaneShotDown(this);
		}
		state = spritestate_t::DEAD;
		GameController::Get().incScore(POINTS_PER_DESTRUCTION[subtype]);
		Explode();
	}
	else {
		GameController::Get().incScore(POINTS_PER_HIT[subtype]);
	}
}

void Enemy::CollisionResult(Sprite *s){
	switch (s->GetType()){
	case spritetype_t::PLAYER_BULLET:
	case spritetype_t::PLAYER:
		if (s->GetType() == spritetype_t::PLAYER && ((Player *)s)->GetMovement() == TUMBLE) return;
		OnPlaneShot();
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