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

void Waves::CreateWaves(const std::string& path){

	/*std::ifstream fin;
	fin.open(path);
	assert(fin.good());

	std::list<std::string> buffer;
	std::string buf;

	while (fin >> buf){
		buffer.push_back(buf);
	}

	for (int i = 0; i < 5; i++){
		buffer.pop_front();
	}

	int i = 0;
	float d = 0;

	for (std::list<std::string>::iterator it = buffer.begin(); it != buffer.end(); ++it){
		enemy_init stats;
		stats.wave = atoi((*it).c_str());
		++it;
		i = 0;
		i = atoi((*it).c_str());
		stats.t = ar[i];
		++it;
		stats.sprite = (*it);
		++it;
		d = 0;
		d = atof((*it).c_str());
		stats.posX = d;
		++it;
		d = 0;
		d = atof((*it).c_str());
		stats.posY = d;
		++it;
		inits.push_back(stats);
	}

	for (std::vector<enemy_init>::iterator it = inits.begin(); it != inits.end(); ++it){

		std::cout << (*it).posX << std::endl;
		std::cout << (*it).posY << std::endl;
		std::cout << (*it).sprite << std::endl;
	
		//Enemy *enemy = new Enemy((*it).posX, (*it).posY, (*it).sprite, (*it).t);
	}*/
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