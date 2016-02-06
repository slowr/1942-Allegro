#include "GameController.h"

GameController GameController::controller;
int GameController::FONT_SIZE = 22;

int CheckPointStarts[3] = {9100, 5450, 1860};
int currCheckPoint = 0;

GameController::GameController(){
	bool checkPointStart = true;
	bool checkPointEnd = false;
	Reset();
}

GameController::~GameController(){
	if (font) al_destroy_font(font);
	if (fontAwesome) al_destroy_font(fontAwesome);
}

GameController& GameController::Get(void){
	return controller;
}

void GameController::bgPositionArgs(float height, float factor) {
	bgHeight = height;
	bgScale = factor;
}

float GameController::getBackgroundY() {
	return backgroundY;
}

void GameController::setBackgroundY(float newY) {

	if (isPlayerDead() == true) {
		backgroundY = backgroundY - 200;
		return;
	}
	backgroundY = newY;
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

bool GameController::getRedraw(void) {
	return redraw;
}

void GameController::setRedraw(bool b) {
	redraw = b;
}

gamestates_t GameController::getGameState(void) {
	return gameState;
}

void GameController::setGameState(gamestates_t state) {
	gameState = state;
}

bool GameController::Respawn(void) {
	if (lives > 0) {
		player->Explode();
		player->SetState(WAIT);
		decLives();

		player->SetX(SCREEN_W / 2);
		player->SetY(SCREEN_H - 100);
		player->SetState(ALIVE);

		return true;
	}
	else if (lives == 0) {
		player->Explode();
		player->SetState(DEAD);
		return false;
	}

	return false;
}

void GameController::disableCheckPoint(void) {
	checkPointEnd = false;
	checkPointStart = false;
}

bool GameController::isCheckPointStart(void) {
	int backbufferY = bgHeight - (SCREEN_H / bgScale) - getBackgroundY();

	if (backbufferY == CheckPointStarts[currCheckPoint]) {
		currCheckPoint++;
		return true;
	}
	return false;
}

bool GameController::isCheckPoint(void) {

	float backbufferY = bgHeight - (SCREEN_H / bgScale) - backgroundY;

	if (backbufferY <= 9100 && backbufferY >= 8900) {
		return true;
	}
	
	if (backbufferY <= 5450 && backbufferY >= 5314) {
		return true;
	}	
	
	if (backbufferY <= 1860 && backbufferY >= 1730) {
		return true;
	}
	return false;
}

bool GameController::isPlayerDead(void) {
	return playerDeath;
}

void GameController::setPlayerCondition(bool condition) {
	playerDeath = condition;
}

void GameController::Reset(void){
	if (font == NULL)
		font = al_load_font("resources/BAUHS93.TTF", FONT_SIZE, 0);
	if (fontAwesome == NULL)
		fontAwesome = al_load_font("resources/fontawesome-webfont.ttf", FONT_SIZE, 0);
	gameState = gamestates_t::PLAYING;
	playerDeath = false;
	redraw = true;
	Score = 0;
	lives = 3;
	tumbles = 3;
	takedowns = 0;
	totalEnemies = 0;
	backgroundY = 0;
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
	redraw = false;
	char *pausedStr = "GAME PAUSED";
	al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_W / 2 - (al_get_text_width(font, pausedStr) / 2), SCREEN_H / 2, 0, pausedStr);
	al_flip_display();
}

void GameController::DeathScreen(void) {
	redraw = false;
	SpriteHolder::Get().DestroyEnemies();
	char *pausedStr = "READY";
	al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_W / 2 - (al_get_text_width(font, pausedStr) / 2), SCREEN_H / 2, 0, pausedStr);
	al_flip_display();
	al_rest(0.5);
	setPlayerCondition(false);
}