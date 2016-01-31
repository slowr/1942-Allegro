#pragma once
//THIS MANAGES THE ENEMIES

#include "types.h"
#include "SpriteHolder.h"

#define MAX_POINTS 50000;		// max player points
#define SPAWN_DELAY 500;		// delay before spawn
#define FIRST 10000;			// first wave worth
#define SECOND 15000;			//
#define THIRD 5000;

class Waves
{
private:
	int waveNo;								// current wave value
	int destroyed;							// % of enemies destroyed
	static Waves waves;
	Waves();
	~Waves();
	void DestroyLastWave();							// destroys last wave
	bool isCheckPoint(timestamp_t time);			// decide if timestamp is checkpoint
public:	
	void SpawnWave(timestamp_t time);				// spawn wave after delay
	static Waves& Waves::Get(void);
	int GetPoints(enemysubtype_t t);				// return destroyed enemy's points
};

