#include "GameController.h"

GameController GameController::controller;

GameController::GameController(){}

GameController::~GameController(){}

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
	Score = 0;
	lives = 3;
	tumbles = 3;
	takedowns = 0;
	totalEnemies = 0;
}