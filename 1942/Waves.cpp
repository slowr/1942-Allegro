#include "Waves.h"
#include "Enemy.h"
#include <sstream>
#include <iostream>
#include <fstream>

Waves Waves::waves;

std::vector<Enemy*> enemies;
std::vector<enemy_init> inits;

enemysubtype_t ar[12] = { GREEN_MONO, GREEN_DOUBLE, GREEN_JET, GRAY_MONO, GRAY_DOUBLE, GRAY_JET, GREEN_MID, GRAY_MID, GREEN_LARGE, GRAY_LARGE, BOSS, RED };
static int point_array[12] = { 30, 70, 30, 50, 50, 50, 1000, 1500, 2000, 2500, 20000, 0 };

void Waves::SpawnWave(timestamp_t curr){

	//if (isCheckPoint(curr)){}
	//else return;

}

void Waves::CreateWaves(){

	
}

bool Waves::isSpawnTime(timestamp_t time){
	
	if (time >= 15806){
		return true;
	}
	else if (time >= 159623){
		return true;
	}
	else if (time >= 303613){
		return true;
	}
	else return false;
}

bool Waves::isCheckPoint(timestamp_t time){ 
	
	if (time > 141385 && time < 159623){
		return true;
	}
	else if (time > 284987 && time < 303613){
		return true;
	}
	else return false;
}

int Waves::GetPoints(enemysubtype_t t){ return point_array[t]; }

void Waves::DestroyLastWave(bool d){
	if (d) SpriteHolder::Get().RemoveByType(spritetype_t::ENEMY);
}

Waves& Waves::Get(void) { return waves; }