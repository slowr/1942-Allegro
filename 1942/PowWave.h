#pragma once

#include "Animator.h"
#include "Enemy.h"

class PowWave
{
private:
	static int NUMBER_OF_PLANES;
	static PowWave instance;

	int PowerUpsSpawned;
	int alivePlanes;
	bool waveAlive;
	bool PowsSpawned[7];
public:
	static PowWave &Get();

	void SpawnWave();
	void SpawnPow(int x, int y);
	PowWave();
	void OnRedPlaneDead(Enemy *e);
	virtual ~PowWave();
};