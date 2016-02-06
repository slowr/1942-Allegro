#include "PowWave.h"
#include "Enemy.h"
#include "PowerUp.h"

int PowWave::NUMBER_OF_PLANES = 5;
PowWave PowWave::instance;

PowWave &PowWave::Get() {
	return instance;
}

PowWave::PowWave() {
	alivePlanes = 0;
	waveAlive = false;
}

PowWave::~PowWave() {

}

void PowWave::SpawnWave() {
	if (!waveAlive && PowerUpsSpawned < 7) {
		std::cout << "Spawning new PowWave." << std::endl;
		Enemy::SpawnPowPlanes(NUMBER_OF_PLANES);
		waveAlive = true;
		alivePlanes = NUMBER_OF_PLANES;
		shotPlanes = 0;
	}
}

void PowWave::SpawnPow(int x, int y) {
	int powNumber;
	if (PowerUpsSpawned == 7) return;
	while (PowsSpawned[(powNumber = rand() % 7)]) { }
	std::cout << "Spawning pow type " << powNumber << std::endl;
	new PowerUp(x, y, (powertype_t)powNumber);
	PowsSpawned[powNumber] = 1;
	PowerUpsSpawned++;
}

void PowWave::OnRedPlaneShotDown(Enemy *e) {
	std::cout << "OnRedPlaneShotDown()" << std::endl;
	if (++shotPlanes == NUMBER_OF_PLANES) {
		SpawnPow(e->GetX(), e->GetY());
	}
}

void PowWave::OnRedPlaneDead(Enemy *e) {
	std::cout << "OnRedPlaneDead()" << std::endl;
	if (--alivePlanes == 0) {
		waveAlive = false;
	}
}