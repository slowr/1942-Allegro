#include "Enemy.h"
#include "PowWave.h"
#include "EnemyBullet.h"
#include "SmallEnemyExplosion.h"
#include "LargeEnemyExplosion.h"
#include "MediumExplosion.h"

const std::string FILMS_BY_TYPE[10] = {
	"green.mono",
	"green.double",
	"green.jet",
	"gray.mono",
	"gray.double",
	"gray.jet",
	"green.mid",
	"gray.mid",
	"green.large",
	"gray.large"
};

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

const int SPEED_PER_TYPE[12] = {
	5,
	5,
	5,
	6,
	6,
	6,
	3,
	4,
	3,
	4,
	5,
	5
};

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



Enemy::Enemy(float _x, float _y, std::string sprite, enemysubtype_t t, std::list<PathEntry*> p) :
Sprite(_x, _y, AnimationFilmHolder::Get().GetFilm(sprite), spritetype_t::ENEMY), subtype(t), health(HEALTH_PER_TYPE[subtype]){
	GameController::Get().incTotalEnemies();
	animation = new MovingPathAnimation(p, 1);
	animator = new MovingPathAnimator();
	animator->Start(this, animation);
	animator->SetOnFinish(OnAnimationFinish, this);
	AnimatorHolder::Register(animator);
	AnimatorHolder::MarkAsRunning(animator);
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

std::list<PathEntry *> Enemy::MediumAnimationInit(int posX, int posY, enemysubtype_t type, int speed, int num) {

	if (posY <= 0) {

		std::list<PathEntry *> p;
		PathEntry * pE;

		if (posX >= SCREEN_W / 2) {

			pE = new PathEntry();
			pE->dx = 0;
			pE->dy = speed;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][BOTTOM]);
			pE->repetitions = (SCREEN_H / 2) / speed;
			pE->action = SHOOT;
			p.push_back(pE);

			doCircle(BOTTOM, p, 100, CLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = 0;
			pE->dy = speed;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][BOTTOM]);
			pE->repetitions = (SCREEN_H / 2) / speed;
			pE->action = SHOOT;
			p.push_back(pE);

			return p;
		}
		else if (posX < SCREEN_W / 2) {

			pE = new PathEntry();
			pE->dx = 0;
			pE->dy = speed;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][BOTTOM]);
			pE->repetitions = (SCREEN_H / 2) / speed;
			pE->action = SHOOT;
			p.push_back(pE);

			doCircle(BOTTOM, p, 100, ANTICLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = 0;
			pE->dy = speed;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][BOTTOM]);
			pE->repetitions = (SCREEN_H / 2) / speed;
			pE->action = SHOOT;
			p.push_back(pE);

			return p;
		}

	}
	else if (posY >= SCREEN_H) {

		std::list<PathEntry *> p;
		PathEntry * pE;

		pE = new PathEntry();
		pE->dx = 0;
		pE->dy = -speed;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][TOP]);
		pE->repetitions = (SCREEN_H) / speed;
		p.push_back(pE);

		return p;
	}
}

std::list<PathEntry *> Enemy::JetAnimationInit(enemysubtype_t type, float _x, float _y) {
	int speed = SPEED_PER_TYPE[type];

	std::list<PathEntry *> p;
	PathEntry * pE;

	if (type == GRAY_JET) {

		int startX = 0;
		int startY = 0;

		int distance;

		int targetX = GameController::Get().getPlayer()->GetX();
		int targetY = GameController::Get().getPlayer()->GetY();

		int repetitions;
		pE = new PathEntry();

		distance = sqrt(pow(targetX, 2) + pow(targetY, 2));
		pE->delay = 25;
		pE->frame = 0;

		repetitions = distance / speed;
		pE->dx = targetX / repetitions;
		pE->dy = targetY / repetitions;
		pE->repetitions = repetitions - 10;

		p.push_back(pE);

		for (int i = 0; i < 4; i++) {
			pE = new PathEntry();

			if (i == 0) {
				pE->action = SHOOT;
			}

			pE->delay = 25;
			pE->frame = i + 1;
			pE->repetitions = 5;
			pE->dx = targetX / repetitions;
			switch (i) {
			case 0:
			case 1:
				pE->dy = targetY / repetitions;
				break;
			case 2:
			case 3:
				pE->dy = -(targetY / repetitions);
				break;
			}
			p.push_back(pE);
		}

		pE = new PathEntry();

		pE->delay = 25;
		pE->frame = 5;

		pE->dx = targetX / repetitions;
		pE->dy = -(targetY / repetitions);
		pE->repetitions = repetitions - 10;

		p.push_back(pE);
		return p;
	}
	else if (type == GREEN_JET) {

		int startX = 0;
		int startY = 0;

		int distance;

		int targetX = GameController::Get().getPlayer()->GetX();
		int targetY = GameController::Get().getPlayer()->GetY();

		int repetitions;
		pE = new PathEntry();

		distance = sqrt(pow(targetX, 2) + pow(targetY, 2));
		pE->delay = 25;
		pE->frame = 0;

		repetitions = distance / speed;
		pE->dx = targetX / repetitions;
		pE->dy = targetY / repetitions;
		pE->repetitions = repetitions - 10;

		p.push_back(pE);

		for (int i = 0; i < 4; i++) {
			pE = new PathEntry();

			if (i == 0) {
				pE->action = SHOOT;
			}

			pE->delay = 25;
			pE->frame = i + 1;
			pE->repetitions = 5;
			pE->dx = targetX / repetitions;
			switch (i) {
			case 0:
			case 1:
				pE->dy = targetY / repetitions;
				break;
			case 2:
			case 3:
				pE->dy = -(targetY / repetitions);
				break;
			}
			p.push_back(pE);
		}

		pE = new PathEntry();

		pE->delay = 25;
		pE->frame = 5;

		pE->dx = targetX / repetitions;
		pE->dy = -(targetY / repetitions);
		pE->repetitions = repetitions - 10;

		p.push_back(pE);
		return p;
	}

	return p;
}

std::list<PathEntry *> Enemy::MonoAnimationInit(enemysubtype_t type, int pX, int speed) {
	int t = rand() % 2;

	if (type == enemysubtype_t::GRAY_MONO && t == 0) {
		std::list<PathEntry *> p;
		PathEntry * pE;

		int startX = 0;
		int startY = 0;

		int distance;

		int targetX = GameController::Get().getPlayer()->GetX();
		int targetY = GameController::Get().getPlayer()->GetY();

		int repetitions;
		pE = new PathEntry();

		distance = sqrt(pow(targetX, 2) + pow(targetY, 2));
		pE->delay = 25;
		pE->frame = 0;

		repetitions = distance / speed;
		pE->dx = targetX / repetitions;
		pE->dy = targetY / repetitions;
		pE->repetitions = repetitions - 10;

		p.push_back(pE);

		for (int i = 0; i < 4; i++) {
			pE = new PathEntry();

			if (i == 0) {
				pE->action = SHOOT;
			}

			pE->delay = 25;
			pE->frame = i + 1;
			pE->repetitions = 5;
			pE->dx = targetX / repetitions;
			switch (i) {
			case 0:
			case 1:
				pE->dy = targetY / repetitions;
				break;
			case 2:
			case 3:
				pE->dy = -(targetY / repetitions);
				break;
			}
			p.push_back(pE);
		}

		pE = new PathEntry();

		pE->delay = 25;
		pE->frame = 5;

		pE->dx = targetX / repetitions;
		pE->dy = -(targetY / repetitions);
		pE->repetitions = repetitions - 10;

		p.push_back(pE);
		return p;
	}
	else if (type == enemysubtype_t::GRAY_MONO && t == 1) {
		std::list<PathEntry *> p;
		PathEntry * pE;

		int startX = 0;
		int startY = 0;

		int distance;

		int targetX = GameController::Get().getPlayer()->GetX();
		int targetY = GameController::Get().getPlayer()->GetY();

		int repetitions;
		pE = new PathEntry();

		distance = sqrt(pow(targetX, 2) + pow(targetY, 2));
		pE->delay = 25;
		pE->frame = 0;

		repetitions = distance / speed;
		pE->dx = targetX / repetitions;
		pE->dy = targetY / repetitions;
		pE->repetitions = repetitions - 10;

		p.push_back(pE);

		for (int i = 0; i < 4; i++) {
			pE = new PathEntry();

			if (i == 0) {
				pE->action = SHOOT;
			}

			pE->delay = 25;
			pE->frame = i + 6;
			pE->repetitions = 5;
			pE->dx = targetX / repetitions;
			switch (i) {
			case 0:
			case 1:
				pE->dy = targetY / repetitions;
				break;
			case 2:
			case 3:
				pE->dy = -(targetY / repetitions);
				break;
			}
			p.push_back(pE);
		}

		pE = new PathEntry();

		pE->delay = 25;
		pE->frame = 5;

		pE->dx = targetX / repetitions;
		pE->dy = -(targetY / repetitions);
		pE->repetitions = repetitions - 10;

		p.push_back(pE);
		return p;
	}

	if (type == enemysubtype_t::GREEN_MONO && pX <= 0) {				// clockwise
		std::list<PathEntry *> p;
		PathEntry * pE;

		int circles = rand() % 5;
		float radius = float(rand() % 120 + 60);
		int loop_start = 10;
		int straight_reps = rand() % 10;

		pE = new PathEntry();
		pE->dx = speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
		pE->repetitions = loop_start;
		pE->action = SHOOT;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(RIGHT, p, radius, CLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = speed;
			pE->dy = 0;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		doCircle(RIGHT, p, radius, CLOCKWISE, 6);

		pE = new PathEntry();
		pE->dx = speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
		pE->repetitions = ((SCREEN_W / 2) / speed) - (loop_start + straight_reps) + 1;
		pE->action = SHOOT;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(RIGHT, p, radius, CLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = speed;
			pE->dy = 0;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		pE = new PathEntry();
		pE->dx = speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
		pE->repetitions = ((SCREEN_W) / speed) - (loop_start + straight_reps) + 1;
		pE->action = SHOOT;
		p.push_back(pE);

		return p;
	}
	else if (type == enemysubtype_t::GREEN_MONO && pX >= SCREEN_W) {	// counter clockwise
		std::list<PathEntry *> p;
		PathEntry * pE;

		int circles = rand() % 5;
		float radius = float(rand() % 120 + 60);
		int loop_start = 10;
		int straight_reps = rand() % 10;

		pE = new PathEntry();
		pE->dx = -speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][LEFT]);
		pE->repetitions = loop_start;
		pE->action = SHOOT;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(LEFT, p, radius, ANTICLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = -speed;
			pE->dy = 0;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][LEFT]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		doCircle(LEFT, p, radius, ANTICLOCKWISE, 6);

		pE = new PathEntry();
		pE->dx = -speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][LEFT]);
		pE->repetitions = ((SCREEN_W / 2) / speed) - (loop_start + straight_reps) + 1;
		pE->action = SHOOT;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(LEFT, p, radius, ANTICLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = -speed;
			pE->dy = 0;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][LEFT]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		pE = new PathEntry();
		pE->dx = -speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][LEFT]);
		pE->repetitions = ((SCREEN_W) / speed) - (loop_start + straight_reps) + 1;
		pE->action = SHOOT;
		p.push_back(pE);

		return p;
	}
}

std::list<PathEntry *> Enemy::DoubleAnimationInit(enemysubtype_t type, int pX) {
	int speed = SPEED_PER_TYPE[type];

	std::list<PathEntry *> p;
	PathEntry* pE;

	if (type == GRAY_DOUBLE && pX >= SCREEN_W / 2) {

		int circles = rand() % 8;
		float radius = float(rand() % 120 + 60);
		int loop_start = 10;
		int straight_reps = rand() % 10;

		pE = new PathEntry();
		pE->dx = 0;
		pE->dy = speed;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][BOTTOM]);
		pE->repetitions = loop_start;
		pE->action = SHOOT;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(BOTTOM, p, radius, ANTICLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = 0;
			pE->dy = speed;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][BOTTOM]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		doCircle(BOTTOM, p, radius, ANTICLOCKWISE, 6);

		pE = new PathEntry();
		pE->dx = 0;
		pE->dy = speed;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][BOTTOM]);
		pE->repetitions = loop_start;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(BOTTOM, p, radius, ANTICLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = 0;
			pE->dy = speed;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][BOTTOM]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		pE = new PathEntry();
		pE->dx = 0;
		pE->dy = speed;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][BOTTOM]);
		pE->repetitions = ((SCREEN_W) / speed) - (loop_start + straight_reps) + 1;
		pE->action = SHOOT;
		p.push_back(pE);

		return p;
	}
	else if (type == GRAY_DOUBLE && pX <= SCREEN_W / 2) {

		int circles = rand() % 8;
		float radius = float(rand() % 120 + 60);
		int loop_start = 10;
		int straight_reps = rand() % 10;

		pE = new PathEntry();
		pE->dx = 0;
		pE->dy = speed;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][TOP]);
		pE->repetitions = loop_start;
		pE->action = SHOOT;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(TOP, p, radius, CLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = 0;
			pE->dy = speed;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][TOP]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		doCircle(TOP, p, radius, CLOCKWISE, 6);

		pE = new PathEntry();
		pE->dx = 0;
		pE->dy = speed;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][TOP]);
		pE->repetitions = loop_start;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(TOP, p, radius, CLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = 0;
			pE->dy = speed;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][TOP]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		pE = new PathEntry();
		pE->dx = 0;
		pE->dy = speed;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][TOP]);
		pE->repetitions = ((SCREEN_W) / speed) - (loop_start + straight_reps) + 1;
		pE->action = SHOOT;
		p.push_back(pE);

		return p;
	}
	else if (type == GREEN_DOUBLE && pX <= 0) {


		int circles = rand() % 8;
		float radius = float(rand() % 120 + 60);
		int loop_start = 10;
		int straight_reps = rand() % 10;

		pE = new PathEntry();
		pE->dx = speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
		pE->repetitions = loop_start;
		pE->action = SHOOT;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(RIGHT, p, radius, CLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = speed;
			pE->dy = 0;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		doCircle(RIGHT, p, radius, CLOCKWISE, 6);

		pE = new PathEntry();
		pE->dx = speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
		pE->repetitions = ((SCREEN_W / 2) / speed) - (loop_start + straight_reps) + 1;
		pE->action = SHOOT;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(RIGHT, p, radius, CLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = speed;
			pE->dy = 0;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		pE = new PathEntry();
		pE->dx = speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][RIGHT]);
		pE->repetitions = ((SCREEN_W) / speed) - (loop_start + straight_reps) + 1;
		pE->action = SHOOT;
		p.push_back(pE);

		return p;
	}
	else if (type == GREEN_DOUBLE && pX >= SCREEN_W) {
		int circles = rand() % 8;
		float radius = float(rand() % 120 + 60);
		int loop_start = 10;
		int straight_reps = rand() % 10;

		pE = new PathEntry();
		pE->dx = -speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][LEFT]);
		pE->repetitions = loop_start;
		pE->action = SHOOT;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(LEFT, p, radius, ANTICLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = -speed;
			pE->dy = 0;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][LEFT]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		doCircle(LEFT, p, radius, ANTICLOCKWISE, 6);

		pE = new PathEntry();
		pE->dx = -speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][LEFT]);
		pE->repetitions = ((SCREEN_W / 2) / speed) - (loop_start + straight_reps) + 1;
		pE->action = SHOOT;
		p.push_back(pE);

		for (int i = 0; i < circles; i++) {
			doCircle(LEFT, p, radius, ANTICLOCKWISE, 6);

			pE = new PathEntry();
			pE->dx = -speed;
			pE->dy = 0;
			pE->delay = delay;
			pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][LEFT]);
			pE->repetitions = loop_start;
			p.push_back(pE);
		}

		pE = new PathEntry();
		pE->dx = -speed;
		pE->dy = 0;
		pE->delay = delay;
		pE->frame = getFrameClockWise(directionDegrees[CLOCKWISE][LEFT]);
		pE->repetitions = ((SCREEN_W) / speed) - (loop_start + straight_reps) + 1;
		pE->action = SHOOT;
		p.push_back(pE);

		return p;
	}

	return p;
}

std::list<PathEntry *> Enemy::LargeAnimationInit() {
	std::list<PathEntry *> p;
	PathEntry * pE;

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
	pE->repetitions = (SCREEN_H) / 5;
	p.push_back(pE);

	return p;
}

void Enemy::SpawnLarge(enemysubtype_t type) {

	if (type == GREEN_LARGE || type == GRAY_LARGE) {
		std::cout << "LARGE SPAWN";
		std::list<PathEntry*> p = LargeAnimationInit();
		new Enemy(100, SCREEN_H, FILMS_BY_TYPE[type], type, p);
	}
}

void Enemy::SpawnSmall(enemysubtype_t type) {

	int spawnX = rand() % (int)SCREEN_W;
	int spawnY = rand() % (int)SCREEN_H;

	switch (type) {
	case GREEN_MONO: // done
	{
		if (spawnX >= SCREEN_W / 2) {
			if (spawnY >= SCREEN_H / 2 || spawnY <= SCREEN_H) {
				spawnY = SCREEN_H / 2;
			}
			std::list<PathEntry*> p = MonoAnimationInit(GREEN_MONO, SCREEN_W, SPEED_PER_TYPE[GREEN_MONO]);
			new Enemy(SCREEN_W, spawnY, FILMS_BY_TYPE[GREEN_MONO], GREEN_MONO, p);
		}
		else {
			if (spawnY >= SCREEN_H / 2 || spawnY <= SCREEN_H) {
				spawnY = SCREEN_H / 2;
			}
			std::list<PathEntry*> p = MonoAnimationInit(GREEN_MONO, 0, SPEED_PER_TYPE[GREEN_MONO]);
			new Enemy(0, spawnY, FILMS_BY_TYPE[GREEN_MONO], GREEN_MONO, p);
		}
	}
	break;
	case GRAY_MONO: // done
	{
		if (spawnX >= SCREEN_W || spawnX <= SCREEN_W) {
			spawnX = SCREEN_W / 2;
		}
		std::list<PathEntry*> p = MonoAnimationInit(GRAY_MONO, 0, SPEED_PER_TYPE[GRAY_MONO]);
		new Enemy(spawnX, 0, FILMS_BY_TYPE[GRAY_MONO], GRAY_MONO, p);
	}
	break;
	case GREEN_DOUBLE:
	{
		if (spawnX >= SCREEN_W / 2) {
			if (spawnY >= SCREEN_H / 2 || spawnY <= SCREEN_H) {
				spawnY = SCREEN_H / 2;
			}
			std::list<PathEntry*> p = DoubleAnimationInit(GREEN_DOUBLE, SCREEN_W);
			new Enemy(SCREEN_W, spawnY, FILMS_BY_TYPE[GREEN_DOUBLE], GREEN_DOUBLE, p);
		}
		else {
			if (spawnY >= SCREEN_H / 2 || spawnY <= SCREEN_H) {
				spawnY = SCREEN_H / 2;
			}
			std::list<PathEntry*> p = DoubleAnimationInit(GREEN_DOUBLE, 0);
			new Enemy(0, spawnY, FILMS_BY_TYPE[GREEN_DOUBLE], GREEN_DOUBLE, p);
		}

	}
	break;
	case GRAY_DOUBLE:
	{
		if (spawnX >= SCREEN_W || spawnX <= SCREEN_W) {
			spawnX = SCREEN_W / 2;
		}
		std::list<PathEntry*> p = DoubleAnimationInit(GRAY_DOUBLE, spawnX);
		new Enemy(spawnX, 0, FILMS_BY_TYPE[GRAY_DOUBLE], GRAY_DOUBLE, p);
	}
	break;
	case GREEN_JET:
	{
		if (spawnX >= SCREEN_W / 2) {
			if (spawnY >= SCREEN_H / 2 || spawnY <= SCREEN_H) {
				spawnY = SCREEN_H / 2;
			}
			std::list<PathEntry*> p = JetAnimationInit(GREEN_JET, SCREEN_W, spawnY);
			new Enemy(SCREEN_W, spawnY, FILMS_BY_TYPE[GREEN_JET], GREEN_JET, p);
		}
		else {
			if (spawnY >= SCREEN_H / 2 || spawnY <= SCREEN_H) {
				spawnY = SCREEN_H / 2;
			}
			std::list<PathEntry*> p = JetAnimationInit(GREEN_JET, 0, spawnY);
			new Enemy(0, spawnY, FILMS_BY_TYPE[GREEN_JET], GREEN_JET, p);
		}
	}
	break;
	case GRAY_JET:
	{
		if (spawnX >= SCREEN_W || spawnX <= SCREEN_W) {
			spawnX = SCREEN_W / 2;
		}
		std::list<PathEntry*> p = JetAnimationInit(GRAY_JET, spawnX, 0);
		new Enemy(spawnX, 0, FILMS_BY_TYPE[GRAY_JET], GRAY_JET, p);
	}
	break;
	}
}

void Enemy::SpawnMedium(enemysubtype_t type) {

	int spawnX = rand() % (int)SCREEN_W;
	int spawnY = rand() % (int)SCREEN_H;

	switch (type) {
	case GREEN_MID:
	{
		if (spawnY < SCREEN_H / 2) {
			if (spawnX >= SCREEN_W || spawnX <= SCREEN_W) {
				spawnX = SCREEN_W / 2;
			}
			std::list<PathEntry*> pM = MediumAnimationInit(spawnX, 0, GREEN_MID, SPEED_PER_TYPE[GREEN_MID], 0);
			new Enemy(spawnX, 0, FILMS_BY_TYPE[GREEN_MID], GREEN_MID, pM);
		}
		else {
			if (spawnX >= SCREEN_W || spawnX <= SCREEN_W) {
				spawnX = SCREEN_W / 2;
			}
			std::list<PathEntry*> pM = MediumAnimationInit(spawnX, SCREEN_H, GREEN_MID, SPEED_PER_TYPE[GREEN_MID], 0);
			new Enemy(spawnX, SCREEN_H, FILMS_BY_TYPE[GREEN_MID], GREEN_MID, pM);
		}

	}
	break;
	case GRAY_MID:
	{
		if (spawnY < SCREEN_H / 2) {
			if (spawnX >= SCREEN_W || spawnX <= SCREEN_W) {
				spawnX = SCREEN_W / 2;
			}
			std::list<PathEntry*> pM = MediumAnimationInit(spawnX, 0, GRAY_MID, SPEED_PER_TYPE[GRAY_MID], 0);
			new Enemy(spawnX, 0, FILMS_BY_TYPE[GRAY_MID], GRAY_MID, pM);
		}
		else {
			if (spawnX >= SCREEN_W || spawnX <= SCREEN_W) {
				spawnX = SCREEN_W / 2;
			}
			std::list<PathEntry*> pM = MediumAnimationInit(spawnX, SCREEN_H, GRAY_MID, SPEED_PER_TYPE[GRAY_MID], 0);
			new Enemy(spawnX, SCREEN_H, FILMS_BY_TYPE[GRAY_MID], GRAY_MID, pM);
		}
	}
	break;
	}
}

enemysubtype_t Enemy::GetSubType(){
	return subtype;
}

void Enemy::Explode(void) {
	switch (subtype) {
	case enemysubtype_t::GREEN_LARGE:
		new LargeEnemyExplosion(x, y, "gray.explosion");
	case enemysubtype_t::GRAY_LARGE:
		new LargeEnemyExplosion(x, y, "large.explosion");
	case enemysubtype_t::GREEN_MID:
		new MediumExplosion(x, y);
	case enemysubtype_t::GRAY_MID:
		new MediumExplosion(x, y);
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