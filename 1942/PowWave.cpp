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
		float y = rand() % ((int)SCREEN_H / 2);
		for (int i = 0; i < NUMBER_OF_PLANES; i++) {
			new Enemy((i + 1) * -40.f, y, std::string("green.mono"), enemysubtype_t::GRAY_MONO);
		}

		waveAlive = true;
		alivePlanes = NUMBER_OF_PLANES;
	}
}

void PowWave::SpawnPow(int x, int y) {
	int powNumber;
	if (PowerUpsSpawned == 7) return;
	while (PowsSpawned[(powNumber = rand() % 7)]) {
		
	}
	std::cout << "Spawning pow type " << powNumber << std::endl;
	new PowerUp(x, y, (powertype_t)powNumber);
	PowsSpawned[powNumber] = 1;
	PowerUpsSpawned++;
}

void PowWave::OnRedPlaneDead(Enemy *e) {
	std::cout << "OnRedPlaneDead()" << std::endl;
	if (--alivePlanes == 0) {
		std::cout << "Last plane at [" << e->GetX() << ", " << e->GetY() << "]" << std::endl;
		if (e->GetX() < SCREEN_W && e->GetY() < SCREEN_H){
			SpawnPow(e->GetX(), e->GetY());
		}
		waveAlive = false;
	}
}