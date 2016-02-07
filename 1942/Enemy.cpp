#include "Enemy.h"
#include "PowWave.h"
#include "EnemyBullet.h"
#include "SmallEnemyExplosion.h"
#include "LargeEnemyExplosion.h"

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
	GameController::Get().incTotalEnemies();
	AnimationInit();
}

/* Red plane constructor */
Enemy::Enemy(float _x, float _y, std::list<PathEntry *> p) :
	Sprite(_x, _y, AnimationFilmHolder::Get().GetFilm("red.plane"), spritetype_t::ENEMY), subtype(RED), health(HEALTH_PER_TYPE[subtype]) {
	GameController::Get().incTotalEnemies();
	animation = new MovingPathAnimation(p, 1);
	animator = new MovingPathAnimator();
	animator->Start(this, animation);
	animator->SetOnFinish(OnAnimationFinish, this);
	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
}

int getFrameAntiClockWise(float degrees) {
	return (int)((360 - degrees) / (360 / 16) + 8) % 16;
}

int getFrameClockWise(float degrees) {
	return (int)((360 - degrees) / (360 / 16)) % 16;
}

/*

=========
CLOCKWISE
=========
RIGHT = 270,
BOTTOM = 360,
LEFT = 90,
TOP = 180

==============
ANTI-CLOCKWISE
==============
RIGHT = 90,
BOTTOM = 180,
LEFT = 270,
TOP = 360

*/

static int directionDegrees[2][4] = {
	{
		270, 360, 90, 180
	},
	{
		90, 180, 270, 360
	}
};

void Enemy::doCircle(enemylook_t direction, std::list<PathEntry *> &p, float radius, circledirection_t cdir, int resolution = 1) {
	int dir = directionDegrees[cdir][direction];
	int (*getFrameFn)(float d);
	
	float px, py;
	float oldDx = 0;
	float oldDy = 0;
	
	bool doBreakWhenDone = false;
	
	delay_t delay = (speed * 360) / (360 / resolution);

	PathEntry *pE;

	if (cdir == CLOCKWISE) {
		getFrameFn = getFrameClockWise;
	}
	else {
		getFrameFn = getFrameAntiClockWise;
		resolution *= -1;
	}

	oldDx = radius*cos(dir*(M_PI / 180)) - radius;
	oldDy = radius*sin(dir*(M_PI / 180));	
	
	for (float degrees = dir;;) {
		if (doBreakWhenDone){
			if (cdir == CLOCKWISE && degrees >= dir - 1) {
				break;
			}
			else if (cdir == ANTICLOCKWISE && degrees <= dir - 1) {
				break;
			}
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
		pE->frame = getFrameFn(degrees);
		p.push_back(pE);
		oldDx = px;
		oldDy = py;
		if (cdir == CLOCKWISE && degrees >= 360) {
			degrees = 0;
			doBreakWhenDone = true;
		} if (cdir == ANTICLOCKWISE && degrees <= 0) {
			degrees = 360;
			doBreakWhenDone = true;
		}
		degrees += resolution;
	}
}

void Enemy::SpawnPowPlanes(int n) {
	float y = (rand() % ((int)SCREEN_H / 2)) + 50;
	int loop_start = (rand() % 20) + 20;
	int straightRepetitions = (rand() % 40) + 50;
	float r1 = (rand() % 60) + 80;
	float r2 = (rand() % 60) + 80;
	int c1 = (rand() % 2);
	int c2 = (rand() % 2);

	for (int i = 0; i < n; i++) {
		std::list<PathEntry *> p = RedAnimationInit(i, loop_start, straightRepetitions, (circledirection_t) c1, r1, (circledirection_t) c2, r2);
		new Enemy((i + 1) * -50.f, y, p);
	}
}

std::list<PathEntry *> Enemy::RedAnimationInit(int planeN, int loop_start, int straight_repetitions, circledirection_t c1, float radius1, circledirection_t c2, float radius2) {
	float x = (planeN + 1) * -50.f;
	std::list<PathEntry *> p;
	PathEntry * pE;

	pE = new PathEntry();
	pE->dx = speed;
	pE->dy = 0;
	pE->delay = delay;
	pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
	pE->repetitions = loop_start;
	p.push_back(pE);

	for (int i = 0; i < planeN; i++) {
		pE = new PathEntry();
		pE->dx = speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
		pE->repetitions = (50 / speed);
		p.push_back(pE);
	}

	doCircle(RIGHT, p, radius1, c1, 6);

	pE = new PathEntry();
	pE->dx = speed;
	pE->dy = 0;
	pE->delay = delay;
	pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
	pE->repetitions = straight_repetitions;
	p.push_back(pE);

	doCircle(RIGHT, p, radius2, c2, 6);

	pE = new PathEntry();
	pE->dx = speed;
	pE->dy = 0;
	pE->delay = delay;
	pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
	pE->repetitions = ((SCREEN_W - x) / speed) - (loop_start + straight_repetitions) + 1;
	p.push_back(pE);

	return p;
}

void Enemy::AnimationInit(){
	std::list<PathEntry *> p;
	PathEntry * pE;
	float distance;

	switch (subtype){
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
	case enemysubtype_t::GREEN_LARGE:
		pE = new PathEntry();
		pE->dx = 0;
		pE->dy = -5;
		pE->delay = delay;
		pE->frame = 0;
		pE->repetitions = (SCREEN_H / 2) / 5;
		p.push_back(pE);

		pE = new PathEntry();
		pE->dx = 3;
		pE->dy = -2.5;
		pE->delay = delay;
		pE->frame = 0;
		pE->repetitions = (SCREEN_W / 2) / 5;
		pE->action = SHOOT;
		p.push_back(pE);

		pE = new PathEntry();
		pE->dx = -5;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = 0;
		pE->repetitions = (SCREEN_W / 4) / 4;
		pE->action = SHOOT;
		p.push_back(pE);

		for (int i = 0; i < 3; i++) {

			pE = new PathEntry();
			pE->dx = 5;
			pE->dy = 0;
			pE->delay = delay;
			pE->frame = 0;
			pE->repetitions = (SCREEN_W / 2) / 4;
			pE->action = SHOOT;
			p.push_back(pE);

			pE = new PathEntry();
			pE->dx = -5;
			pE->dy = 0;
			pE->delay = delay;
			pE->frame = 0;
			pE->repetitions = (SCREEN_W / 2) / 4;
			pE->action = SHOOT;
			p.push_back(pE);

		}
		pE = new PathEntry();
		pE->dx = 0;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = 0;
		pE->repetitions = 10;
		pE->action = SHOOT;
		p.push_back(pE);

		pE = new PathEntry();
		pE->dx = 0;
		pE->dy = -5;
		pE->delay = delay;
		pE->frame = 0;
		pE->repetitions = (SCREEN_H - GetY()) / 5;
		p.push_back(pE);

		animation = new MovingPathAnimation(p, 2);
		animator = new MovingPathAnimator();
		animator->Start(this, animation);
		animator->SetOnFinish(OnAnimationFinish, this);
		AnimatorHolder::Register(animator);
		AnimatorHolder::MarkAsRunning(animator);
		break;
	default:
		doCircle(BOTTOM, p, 100, CLOCKWISE);

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
	switch (subtype) {
	case enemysubtype_t::GREEN_LARGE:
		new LargeEnemyExplosion(x, y);
	default:
		new SmallEnemyExplosion(x, y);
	}
	
}

void Enemy::OnPlaneShot(void) {
	if (--health == 0) {
		GameController::Get().incTakedowns();
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
		if (s->GetType() == spritetype_t::PLAYER) {
			Player *player = (Player *)s;
			if (player->GetMovement() == LOOP || player->GetMovement() == TAKEOFF || player->GetMovement() == LANDING) return;
		}
		OnPlaneShot();
		break;
	}
}

void Enemy::AnimationFinish(void){
	LatelyDestroyable::Add(this);
}

void Enemy::shoot(){
	if (GameController::Get().GetNoEnemyBulletsPow() && subtype <= enemysubtype_t::GRAY_MID) return;
	if (subtype == GREEN_LARGE) {
		new EnemyBullet(GetX() + frameBox.w / 2, GetY() + frameBox.h * 2);		
	}
	else
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