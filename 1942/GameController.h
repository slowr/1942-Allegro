#pragma once
#include "Player.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

class GameController {
	static int FONT_SIZE;
	Player * player;
	float Score;
	int lives;
	int tumbles;
	int takedowns;
	int totalEnemies;
	static GameController controller;
	GameController();
	~GameController();
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *fontAwesome;
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
	void DrawUI(void);
};

