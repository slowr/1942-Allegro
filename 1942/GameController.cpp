#include "GameController.h"
#include "GameMenu.h"

GameController GameController::controller;
int GameController::FONT_SIZE = 22;
int GameController::NO_ENEMY_BULLETS_POWERUP_DURATION = 5000;
int GameController::CHECKPOINTS[2] = { 5480, 1860 };

int CHECKPOINTS_NUMBER = 2;

GameController::GameController(){
	Reset();
}

GameController::~GameController(){
	if (font) al_destroy_font(font);
	if (fontAwesome) al_destroy_font(fontAwesome);
	if (fontHuge) al_destroy_font(fontHuge);
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
	backgroundY = newY;
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

void GameController::decLoops(){
	loops--;
}

void GameController::incLoops(){
	loops++;
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

int GameController::getLoops(void){
	return loops;
}

int GameController::getTakedowns(void){
	return takedowns;
}

bool GameController::isCheckPoint(void) {
	if (currentCheckPoint >= CHECKPOINTS_NUMBER) return false;

	float backbufferY = bgHeight - (SCREEN_H / bgScale) - backgroundY;
	int error = 40;

	if (backbufferY >= CHECKPOINTS[currentCheckPoint] - error
		&& backbufferY <= CHECKPOINTS[currentCheckPoint] + error){
		currentCheckPoint++;
		return true;
	}

	return false;
}

void GameController::Reset(void){
	if (font == NULL)
		font = al_load_font("resources/BAUHS93.TTF", FONT_SIZE, 0);
	if (fontAwesome == NULL)
		fontAwesome = al_load_font("resources/fontawesome-webfont.ttf", FONT_SIZE, 0);
	if (fontHuge == NULL)
		fontHuge = al_load_font("resources/BAUHS93.TTF", FONT_SIZE * 3, 0);
	gameState = gamestates_t::PLAYING;
	playerDeath = false;
	redraw = true;
	Score = 0;
	lives = 3;
	loops = 3;
	takedowns = 0;
	totalEnemies = 0;
	backgroundY = 0;
	deathTimestamp = checkPointTimestamp = 0;
	currentCheckPoint = 0;
}

void GameController::DrawUI(void) {
	int temp_highscore = 999999;
	
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 5, 0, ALLEGRO_ALIGN_CENTRE, "1UP");
	al_draw_text(font, al_map_rgb(200, 200, 255), SCREEN_W / 5, 25, ALLEGRO_ALIGN_CENTRE, std::to_string((int)Score).c_str());

	al_draw_text(font, al_map_rgb(255, 255, 0), SCREEN_W / 2, 0, ALLEGRO_ALIGN_CENTRE, "HIGH SCORE");
	al_draw_text(font, al_map_rgb(200, 200, 255), SCREEN_W / 2, 25, ALLEGRO_ALIGN_CENTRE, std::to_string((int)temp_highscore).c_str());

	al_draw_text(font, al_map_rgb(255, 255, 255), 4*(SCREEN_W / 5), 0, ALLEGRO_ALIGN_CENTRE, "2UP");
	al_draw_text(font, al_map_rgb(200, 200, 255), 4 * (SCREEN_W / 5), 25, ALLEGRO_ALIGN_CENTRE, std::to_string((int)0).c_str());
	

	std::string tumbleString(loops, 'R');

	uint16_t *uStr = new uint16_t[max(lives, loops) + 1];

	for (int i = 0; i < lives; i++) {
		uStr[i] = u'\uF004';
	}
	uStr[lives] = 0;

	ALLEGRO_USTR* lives_string = al_ustr_new_from_utf16(uStr);

	for (int i = 0; i < loops; i++) {
		uStr[i] = u'\uF0E2';
	}
	uStr[loops] = 0;

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

void GameController::Respawn(void) {
	player->SetX(SCREEN_W / 2 - (player->GetFrameBox(0).w * ScaleFactor) / 2);
	player->SetY(SCREEN_H - 100);
	player->setDead(false);
}

void GameController::DeathScreen(void) {
	char *pausedStr;
	if (lives > 0) pausedStr = "GET READY!";
	else pausedStr = "GAME OVER!";

	long timestamp = TIMESTAMP(tickCount);

	if (deathTimestamp == 0) {
		SpriteHolder::Get().DestroyEnemies(false);
		SpriteHolder::Get().DestroyPows();
		deathTimestamp = timestamp;
	}

	if (lives == 0 || (timestamp - deathTimestamp) % 600 <= 400){
		al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_W / 2 - (al_get_text_width(font, pausedStr) / 2), SCREEN_H / 2 - 80, 0, pausedStr);
	}

	if (lives > 0){
		int diffInSec = 5 - (timestamp - deathTimestamp) / 1000;
		std::string countdownString(1, (char) ('0' + diffInSec));
		al_draw_text(fontHuge, al_map_rgb(255, 255, 255), SCREEN_W / 2 - (al_get_text_width(fontHuge, countdownString.c_str()) / 2), SCREEN_H / 2 - 40, 0, countdownString.c_str());
	}

	if (timestamp - deathTimestamp >= 5000) {
		deathTimestamp = 0;
		if (lives > 0) {
			Respawn();
		} else {
			setGameState(MENU);
			menu->ShowMenu();
		}
	}
}

void GameController::CheckPointScreen() {
	long timestamp = TIMESTAMP(tickCount);

	if (checkPointTimestamp == 0) {
		SpriteHolder::Get().DestroyEnemies(false);
		SpriteHolder::Get().DestroyPows();
		checkPointTimestamp = timestamp;
	}

	std::string checkPointStr("CHECKPOINT " + std::to_string(currentCheckPoint));
	std::string planesSpawnedStr("TOTAL PLANES");
	std::string planesDestroyedStr("PLANES DESTROYED");
	std::string percentageStr("PERCENTAGE");
	std::string pointsStr("TOTAL POINTS");

	std::string totalPlanes = std::to_string(totalEnemies);
	std::string destroyedPlanes = std::to_string(takedowns);
	std::string percentage = std::to_string((int) (((float) takedowns / (float) totalEnemies) * 100.f)) + "%";
	std::string points = std::to_string((int) Score);

	al_clear_to_color(al_map_rgb(0, 125, 235));

	al_draw_text(fontHuge, al_map_rgb(255, 255, 255), SCREEN_W / 2 - (al_get_text_width(fontHuge, checkPointStr.c_str()) / 2), 150, 0, checkPointStr.c_str());

	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 150, 250, ALLEGRO_ALIGN_LEFT, planesSpawnedStr.c_str());
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 150, 280, ALLEGRO_ALIGN_LEFT, planesDestroyedStr.c_str());
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 150, 310, ALLEGRO_ALIGN_LEFT, percentageStr.c_str());
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 150, 350, ALLEGRO_ALIGN_LEFT, pointsStr.c_str());

	al_draw_text(font, al_map_rgb(235, 110, 0), SCREEN_W / 2 + 150, 250, ALLEGRO_ALIGN_RIGHT, totalPlanes.c_str());
	al_draw_text(font, al_map_rgb(235, 110, 0), SCREEN_W / 2 + 150, 280, ALLEGRO_ALIGN_RIGHT, destroyedPlanes.c_str());
	al_draw_text(font, al_map_rgb(235, 110, 0), SCREEN_W / 2 + 150, 310, ALLEGRO_ALIGN_RIGHT, percentage.c_str());
	al_draw_text(font, al_map_rgb(255, 255, 0), SCREEN_W / 2 + 150, 350, ALLEGRO_ALIGN_RIGHT, points.c_str());

	if (timestamp - checkPointTimestamp >= 8000) {
		checkPointTimestamp = 0;
		player->TakeOff();
	}
}

void GameController::SetMenu(GameMenu *m) {
	menu = m;
}

GameMenu *GameController::GetMenu(void) {
	return menu;
}