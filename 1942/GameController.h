#pragma once
#include "Player.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

class GameMenu;

class GameController {
	static int NO_ENEMY_BULLETS_POWERUP_DURATION;
	static int FONT_SIZE;
	static int CHECKPOINTS[2];
	Player * player;
	bool playerDeath;
	float backgroundY;
	float bgHeight;
	float bgScale;
	bool redraw;
	float Score;
	int lives;
	int loops;
	int takedowns;
	int totalEnemies;
	int currentCheckPoint;
	long deathTimestamp;
	long checkPointTimestamp;
	bool noEnemyBullets;
	GameMenu *menu;
	gamestates_t gameState;
	timestamp_t noEnemyBulletsStart;
	static GameController controller;
	GameController();
	~GameController();
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *fontAwesome;
	ALLEGRO_FONT *fontHuge;
public:
	static GameController& Get(void);
	void SetPlayer(Player * p);
	void SetMenu(GameMenu *m);
	GameMenu *GetMenu(void);
	void incScore(float s);
	void decLives();
	void incLives();
	void decLoops();
	void incLoops();
	void incTakedowns();
	void incTotalEnemies();

	bool getRedraw(void);
	void setRedraw(bool b);
	float getBackgroundY();
	void setBackgroundY(float newY);
	gamestates_t getGameState(void);
	void setGameState(gamestates_t state);
	bool isCheckPoint(void);
	void bgPositionArgs(float height, float factor);

	Player * getPlayer(void);
	float getScore(void);
	int getLives(void);
	int getLoops(void);
	int getTakedowns(void);

	void Respawn(void);
	void Reset(void);
	void DrawUI(void);
	void DrawPaused(void);
	void DeathScreen(void);
	void CheckPointScreen();

	void SetNoEnemyBulletsPow(bool val);
	bool GetNoEnemyBulletsPow();
};

