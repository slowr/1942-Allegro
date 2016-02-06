#pragma once
#include "Player.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

class GameController {
	static int NO_ENEMY_BULLETS_POWERUP_DURATION;
	static int FONT_SIZE;
	Player * player;
	bool playerDeath;
	float backgroundY;
	float bgHeight;
	float bgScale;
	bool redraw;
	float Score;
	int lives;
	int tumbles;
	int takedowns;
	int totalEnemies;
	bool noEnemyBullets;
	gamestates_t gameState;
	timestamp_t noEnemyBulletsStart;
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

	bool getRedraw(void);
	void setRedraw(bool b);
	float getBackgroundY();
	void setBackgroundY(float newY);
	gamestates_t getGameState(void);
	void setGameState(gamestates_t state);
	bool isPlayerDead(void);
	void setPlayerCondition(bool condition);
	void disableCheckPoint(void);
	bool isCheckPoint(void);
	bool isCheckPointStart(void);
	void bgPositionArgs(float height, float factor);

	Player * getPlayer(void);
	float getScore(void);
	int getLives(void);
	int getTumbles(void);
	int getTakedowns(void);

	bool Respawn(void);
	void Reset(void);
	void DrawUI(void);
	void DrawPaused(void);
	void DeathScreen(void);

	void SetNoEnemyBulletsPow(bool val);
	bool GetNoEnemyBulletsPow();
};

