#pragma once

#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "AnimatorHolder.h"
#include "MovingPathAnimation.h"
#include "MovingPathAnimator.h"
#include "SpriteHolder.h"
#include "PlayerBullet.h"
#include "LatelyDestroyable.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Player.h"
#include "GameController.h"

class Enemy : public Sprite {
public:
	enum enemylook_t {
		RIGHT,
		BOTTOM,
		LEFT,
		TOP
	};

	enum circledirection_t {
		CLOCKWISE,
		ANTICLOCKWISE
	};
private:
	const static int speed = 5;
	const static int delay = 15;
	enemysubtype_t subtype;
	Animator * animator;
	Animation * animation;
	timestamp_t last_timestamp;
	int health;

	static std::list<PathEntry *> MediumAnimationInit(int posX, int posY, enemysubtype_t type, int speed, int num);
	static std::list<PathEntry *> JetAnimationInit(enemysubtype_t type, float _x, float _y);
	static std::list<PathEntry *> MonoAnimationInit(enemysubtype_t type, int pX, int speed);
	static std::list<PathEntry *> DoubleAnimationInit(enemysubtype_t type, int pX);
	static std::list<PathEntry *> LargeAnimationInit();
	static std::list<PathEntry *> RedAnimationInit(int planeN, int loop_start, int straight_repetitions, circledirection_t c1, float radius1, circledirection_t c2, float radius2);
	static void doCircle(enemylook_t direction, std::list<PathEntry*>& p, float radius, circledirection_t cdir, int resolution);
public:
	enemysubtype_t GetSubType();

	static void SpawnLarge(enemysubtype_t type);
	static void SpawnSmall(enemysubtype_t type);
	static void SpawnMedium(enemysubtype_t type);
	Enemy(float _x, float _y, std::string sprite, enemysubtype_t t, std::list<PathEntry*> p);
	Enemy(float _x, float _y, std::list<PathEntry *> p);
	virtual void CollisionResult(Sprite *s);
	virtual void AnimationFinish(void);
	void shoot();
	void OnPlaneShot(void);
	void Explode(void);
	static void SpawnPowPlanes(int n);
	~Enemy(void);
};