#pragma once

#include "types.h"
#include <string>
#include "GameController.h"
#include "Enemy.h"

#define MAX_PLANES 10
//#define SMALL_DELAY 

class RegularWave
{
private:
	static RegularWave reg_wave;
	
	static const timestamp_t spawnDelay = 2000;
	
	unsigned long lastSpawn;

	const unsigned long smallDelay = 1000;
	const unsigned long mediumDelay = 10000;
	const unsigned long largeDelay = 50000;

	unsigned long lastSmallSpawn;
	unsigned long lastMediumSpawn;
	unsigned long lastLargeSpawn;

	int aliveNum;
	int deadNum;
	int spawnCount;
	bool newSpawn;

public:
	void OnEnemyDeath();
	void SetSpawnTime(unsigned long tick);
	static enemysubtype_t GetRandomSubType(const std::string size);
	int GetKillPercentage();
	static RegularWave &Get();
	void SpawnRegular();
	RegularWave();
	~RegularWave();
};

