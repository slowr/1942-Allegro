#include "RegularWave.h"
#include "PowWave.h"

const int SPAWN_FREQUENCY[10] = {
	100,
	100,
	100,
	100,
	100,
	100,
	400,
	400,
	1000,
	1000
};

const enemysubtype_t SMALL[6] = {
	GREEN_MONO,
	GREEN_DOUBLE,
	GREEN_JET,
	GRAY_MONO,
	GRAY_DOUBLE,
	GRAY_JET
};

const enemysubtype_t MEDIUM[2] = {
	GREEN_MID,
	GRAY_MID
};

const enemysubtype_t LARGE[2] = {
	GREEN_LARGE,
	GRAY_LARGE
};

RegularWave RegularWave::reg_wave;

RegularWave::RegularWave()
{
	aliveNum = 0;

	lastSpawn = tickCount;
}

void RegularWave::Reset() {
	lastSpawn = 0;
}

RegularWave::~RegularWave()
{
}


RegularWave &RegularWave::Get() {
	return reg_wave;
}

void RegularWave::SpawnRegular() {
	int spawnChance = rand() % 10;


	if (lastSpawn == 0 || tickCount >= lastSpawn + spawnDelay) {

		if (tickCount >= lastLargeSpawn + largeDelay) {
			Enemy::SpawnLarge(GetRandomSubType("large"));
			aliveNum++;
			lastSpawn = tickCount;
			lastLargeSpawn = tickCount;
		}

		if (tickCount >= lastMediumSpawn + mediumDelay) {
			PowWave::Get().SpawnWave();
			aliveNum++;
			lastSpawn = tickCount;
			lastMediumSpawn = tickCount;
		}

		switch (spawnChance) {
		case 0:
			Enemy::SpawnSmall(GetRandomSubType("small"));
			aliveNum++;
			lastSpawn = tickCount;
			break;
		case 1:
			Enemy::SpawnSmall(GetRandomSubType("small"));
			aliveNum++;
			lastSpawn = tickCount;
			break;
		case 2:
			Enemy::SpawnMedium(GetRandomSubType("medium"));
			aliveNum++;
			lastSpawn = tickCount;
			break;
		case 3:
			Enemy::SpawnSmall(GetRandomSubType("small"));
			aliveNum++;
			lastSpawn = tickCount;
			break;
		case 4:
			Enemy::SpawnSmall(GetRandomSubType("small"));
			aliveNum++;
			lastSpawn = tickCount;
			break;
		case 5:
			Enemy::SpawnSmall(GetRandomSubType("small"));
			aliveNum++;
			lastSpawn = tickCount;
			break;
		case 6:
			Enemy::SpawnSmall(GetRandomSubType("small"));
			aliveNum++;
			lastSpawn = tickCount;
			break;
		case 7:
			Enemy::SpawnSmall(GetRandomSubType("small"));
			aliveNum++;
			lastSpawn = tickCount;
			break;
		case 8:
			Enemy::SpawnSmall(GetRandomSubType("small"));
			aliveNum++;
			lastSpawn = tickCount;
			break;
		case 9:
			Enemy::SpawnSmall(GetRandomSubType("small"));
			aliveNum++;
			lastSpawn = tickCount;
			break;
		}
	}
}

void RegularWave::SetSpawnTime(unsigned long tick) {
	lastSpawn = tick;
}

void RegularWave::OnEnemyDeath() {
	aliveNum--;
	deadNum++;
}

int RegularWave::GetKillPercentage() {
	return (deadNum / spawnCount)*100;
}

enemysubtype_t RegularWave::GetRandomSubType(const std::string size) {

	if (size.compare("small") == 0) {
		int s = rand() % 6;
		return SMALL[s];
	}
	else if (size.compare("medium") == 0) {
		int m = rand() % 2;
		return MEDIUM[m];
	}
	else if (size.compare("large") == 0) {
		int l = rand() % 2;
		return LARGE[l];
	}
}