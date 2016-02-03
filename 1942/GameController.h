#pragma once
#include "Player.h"


class GameController {
	Player * player;
	float Score;
	int lives;
	int tumbles;
	int takedowns;
	int totalEnemies;
	static GameController controller;
	GameController();
	~GameController();
public:
	static GameController& Get(void);
	void SetPlayer(Player * p);
	void incScore(float s);
	void decLives();
	void incLives();
	void decTumbles();
	void incTumbles();
	void incTakedowns();
	void incTotalEnemies();

	Player * getPlayer(void);
	float getScore(void);
	int getLives(void);
	int getTumbles(void);
	int getTakedowns(void);

	void Reset(void);
};

