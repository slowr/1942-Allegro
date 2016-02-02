#pragma once
//THIS MANAGES THE ENEMIES

#include "types.h"
#include "SpriteHolder.h"

#define MAX_POINTS 50000;		// max player points
#define FIRST 10000;			// first wave worth
#define SECOND 15000;			//
#define THIRD 5000;

struct enemy_init{
	unsigned wave;
	enemysubtype_t t;
	std::string sprite;
	float posX;
	float posY;
};

class Waves
{
private:
	int waveNo;								// current wave value
	int destroyed;							// % of enemies destroyed
	static Waves waves;
	bool isSpawnTime(timestamp_t time);
public:
	void CreateWaves();		// initializes waves
	void DestroyLastWave(bool d);							// destroys last wave
	bool isCheckPoint(timestamp_t time);			// decide if timestamp is checkpoint
	void SpawnWave(timestamp_t time);				// spawn wave after delay
	static Waves& Waves::Get(void);
	int GetPoints(enemysubtype_t t);				// return destroyed enemy's points
};

