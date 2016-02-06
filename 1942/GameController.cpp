#include "GameController.h"

GameController GameController::controller;
int GameController::FONT_SIZE = 22;
int GameController::NO_ENEMY_BULLETS_POWERUP_DURATION = 5000;

GameController::GameController(){
	Reset();
}

GameController::~GameController(){
	if (font) al_destroy_font(font);
	if (fontAwesome) al_destroy_font(fontAwesome);
}

GameController& GameController::Get(void){
	return controller;
}

void GameController::SetPlayer(Player * p){
	player = p;
}

void GameController::incScore(float s){
	Score += s;
}

void GameController::decLives(){
	lives--;
}

void GameController::incLives(){
	lives++;
}

void GameController::decTumbles(){
	tumbles--;
}

void GameController::incTumbles(){
	tumbles++;
}

void GameController::incTakedowns(){
	takedowns++;
}

void GameController::incTotalEnemies(){
	totalEnemies++;
}

Player *  GameController::getPlayer(){
	return player;
}

float GameController::getScore(void){
	return Score;
}

int GameController::getLives(void){
	return lives;
}

int GameController::getTumbles(void){
	return tumbles;
}

int GameController::getTakedowns(void){
	return takedowns;
}

void GameController::Reset(void){
	if (font == NULL)
		font = al_load_font("resources/BAUHS93.TTF", FONT_SIZE, 0);
	if (fontAwesome == NULL)
		fontAwesome = al_load_font("resources/fontawesome-webfont.ttf", FONT_SIZE, 0);
	Score = 0;
	lives = 3;
	tumbles = 3;
	takedowns = 0;
	totalEnemies = 0;
}

void GameController::DrawUI(void) {
	int temp_highscore = 999999;
	
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 5, 0, ALLEGRO_ALIGN_CENTRE, "1UP");
	al_draw_text(font, al_map_rgb(200, 200, 255), SCREEN_W / 5, 25, ALLEGRO_ALIGN_CENTRE, std::to_string((int)Score).c_str());

	al_draw_text(font, al_map_rgb(255, 255, 0), SCREEN_W / 2, 0, ALLEGRO_ALIGN_CENTRE, "HIGH SCORE");
	al_draw_text(font, al_map_rgb(200, 200, 255), SCREEN_W / 2, 25, ALLEGRO_ALIGN_CENTRE, std::to_string((int)temp_highscore).c_str());

	al_draw_text(font, al_map_rgb(255, 255, 255), 4*(SCREEN_W / 5), 0, ALLEGRO_ALIGN_CENTRE, "2UP");
	al_draw_text(font, al_map_rgb(200, 200, 255), 4 * (SCREEN_W / 5), 25, ALLEGRO_ALIGN_CENTRE, std::to_string((int)0).c_str());
	

	std::string tumbleString(tumbles, 'R');

	uint16_t *uStr = new uint16_t[max(lives, tumbles) + 1];

	for (int i = 0; i < lives; i++) {
		uStr[i] = u'\uF004';
	}
	uStr[lives] = 0;

	ALLEGRO_USTR* lives_string = al_ustr_new_from_utf16(uStr);

	for (int i = 0; i < tumbles; i++) {
		uStr[i] = u'\uF0E2';
	}
	uStr[tumbles] = 0;

	ALLEGRO_USTR* tumbles_string = al_ustr_new_from_utf16(uStr);
	
	delete uStr;

	al_draw_ustr(fontAwesome, al_map_rgb(255, 0, 0), SCREEN_W - al_get_ustr_width(fontAwesome, tumbles_string) - 5, SCREEN_H - 25, 0, tumbles_string);
	al_draw_ustr(fontAwesome, al_map_rgb(255, 0, 0), 5, SCREEN_H - 25, 0, lives_string);
	
	al_ustr_free(lives_string);
	al_ustr_free(tumbles_string);
}

void GameController::DrawPaused(void) {
	char *pausedStr = "GAME PAUSED";
	al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_W / 2 - (al_get_text_width(font, pausedStr) / 2), SCREEN_H / 2, 0, pausedStr);
	al_flip_display();
}

void GameController::SetNoEnemyBulletsPow(bool val) {
	noEnemyBullets = val;
	if (val) {
		noEnemyBulletsStart = TIMESTAMP(tickCount);
	}
	else {
		noEnemyBulletsStart = 0;
	}
}

bool GameController::GetNoEnemyBulletsPow() {
	if (noEnemyBullets && TIMESTAMP(tickCount) - noEnemyBulletsStart < NO_ENEMY_BULLETS_POWERUP_DURATION)
		return true;
	return (noEnemyBullets = false);
}