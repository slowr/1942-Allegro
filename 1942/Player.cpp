#include "Player.h"
#include "GameController.h"
#include "PlayerExplosion.h"
#include "PlayerBullet.h"
#include "RegularWave.h"

void Player::movementAnimatorCallback(Animator *a, void *c){
	LatelyDestroyable::Add(a);
}

void Player::reverseAnimatorCallback(Animator *a, void *c){
	LatelyDestroyable::Add(a);
	((Player *)c)->SetFrame(0);
}

void Player::loopAnimatorCallback(Animator *a, void *c){
	((Player *)c)->movement = NONE;
}

void Player::takeoffAnimatorCallback(Animator *a, void *c) {
	((Player *)c)->movement = NONE;
}

void Player::landingAnimatorCallback(Animator *a, void *c) {
	((Player *)c)->movement = LANDED;
}

Player::Player(void) : Sprite(SCREEN_W / 2 - AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).w*ScaleFactor / 2, SCREEN_H / 2, AnimationFilmHolder::Get().GetFilm("player.sprite"), spritetype_t::PLAYER), movement(NONE), dead(false), hasSideFighters(false) {

	this->SetFrame(22);

	shotSound = al_load_sample("resources/shot.ogg");

	leftAnimation = new FrameRangeAnimation(4, 6, 0, 0, delay, false, 1);
	leftAnimator = new FrameRangeAnimator();
	leftAnimator->SetOnFinish(movementAnimatorCallback, this);

	rightAnimation = new FrameRangeAnimation(1, 3, 0, 0, delay, false, 1);
	rightAnimator = new FrameRangeAnimator();
	rightAnimator->SetOnFinish(movementAnimatorCallback, this);

	revleftAnimation = new FrameRangeAnimation(6, 4, 0, 0, delay, false, 1);
	revleftAnimator = new FrameRangeAnimator();
	revleftAnimator->SetOnFinish(reverseAnimatorCallback, this);

	revrightAnimation = new FrameRangeAnimation(3, 1, 0, 0, delay, false, 1);
	revrightAnimator = new FrameRangeAnimator();
	revrightAnimator->SetOnFinish(reverseAnimatorCallback, this);

	PathEntry * pE;
	for (int i = 0; i < 15; i++){
		pE = new PathEntry();
		pE->dx = 0;
		switch (i){
		case 0:
		case 1:
		case 2:
		case 12:
		case 13:
		case 14:
			pE->dy = -10;
			break;
		case 3:
		case 11:
			pE->dy = 0;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			pE->dy = 10;
			break;
		}
		pE->delay = 120;
		pE->frame = i + 7;
		pE->repetitions = 1;
		loopList.push_back(pE);
	}

	pE = new PathEntry();
	pE->dx = 0;
	pE->dy = -10;
	pE->delay = 120;
	pE->repetitions = 1;
	pE->frame = 0;

	loopList.push_back(pE);

	loopAnimation = new MovingPathAnimation(std::list<PathEntry *>(loopList), 1337);
	loopAnimator = new MovingPathAnimator();
	loopAnimator->SetOnFinish(loopAnimatorCallback, this);

	PathEntry *takeoff = new PathEntry();

	takeoff->dx = 0;
	takeoff->dy = -10;
	takeoff->delay = 120;
	takeoff->repetitions = 10;
	takeoff->frame = 22;

	takeoffList.push_back(takeoff);

	takeoff = new PathEntry();

	takeoff->dx = 0;
	takeoff->dy = -10;
	takeoff->delay = 120;
	takeoff->repetitions = 1;
	takeoff->frame = 23;

	takeoffList.push_back(takeoff);

	takeoff = new PathEntry();

	takeoff->dx = 0;
	takeoff->dy = -10;
	takeoff->delay = 120;
	takeoff->repetitions = 1;
	takeoff->frame = 24;

	takeoffList.push_back(takeoff);

	for (int i = 0; i < 15; i++) {
		takeoff = new PathEntry();
		takeoff->dx = 0;
		switch (i) {
		case 0:
		case 1:
		case 2:
		case 12:
		case 13:
		case 14:
			takeoff->dy = -10;
			break;
		case 3:
		case 11:
			takeoff->dy = 0;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			takeoff->dy = 10;
			break;
		}
		takeoff->delay = 120;
		takeoff->frame = i + 7;
		takeoff->repetitions = 1;
		takeoffList.push_back(takeoff);
	}

	takeoff = new PathEntry();
	takeoff->dx = 0;
	takeoff->dy = +10;
	takeoff->delay = 120;
	takeoff->repetitions = 10;
	takeoff->frame = 0;

	takeoffList.push_back(takeoff);

	takeoffAnimation = new MovingPathAnimation(std::list<PathEntry *>(takeoffList), 15);
	takeoffAnimator = new MovingPathAnimator();
	takeoffAnimator->SetOnFinish(takeoffAnimatorCallback, this);

	PathEntry *landing = new PathEntry();

	landing->dx = 0;
	landing->dy = 0;
	landing->delay = 120;
	landing->repetitions = 0;
	landing->frame = 0;

	landingList.push_back(landing);

	landing = new PathEntry();
	landing->dx = 0;
	landing->dy = -10;
	landing->delay = 120;
	landing->repetitions = 1;
	landing->frame = 24;

	landingList.push_back(landing);

	landing = new PathEntry();
	landing->dx = 0;
	landing->dy = -10;
	landing->delay = 120;
	landing->repetitions = 1;
	landing->frame = 23;

	landingList.push_back(landing);

	landing = new PathEntry();
	landing->dx = 0;
	landing->dy = -10;
	landing->delay = 120;
	landing->repetitions = 1;
	landing->frame = 22;

	landingList.push_back(landing);

	landingAnimation = new MovingPathAnimation(std::list<PathEntry *>(landingList), 16);
	landingAnimator = new MovingPathAnimator();
	landingAnimator->SetOnFinish(landingAnimatorCallback, this);

	AnimatorHolder::Register(leftAnimator);
	AnimatorHolder::Register(rightAnimator);
	AnimatorHolder::Register(revleftAnimator);
	AnimatorHolder::Register(revrightAnimator);
	AnimatorHolder::Register(loopAnimator);
	AnimatorHolder::Register(takeoffAnimator);
	AnimatorHolder::Register(landingAnimator);

	last_timestamp = 0;
}

void Player::StopMoving(){
	if (movement == NONE || movement == LOOP || movement == TAKEOFF || movement == LANDING || movement == LANDED) return;
	if (movement == LEFT){
		AnimatorHolder::MarkAsRunning(revleftAnimator);
		revleftAnimator->Start(this, revleftAnimation);
	}
	else{
		AnimatorHolder::MarkAsRunning(revrightAnimator);
		revrightAnimator->Start(this, revrightAnimation);
	}
	movement = NONE;

	AnimatorHolder::MarkAsSuspended(leftAnimator);
	AnimatorHolder::MarkAsSuspended(rightAnimator);
}

void Player::MoveLeft(){
	if (movement == LEFT || movement == LOOP || movement == TAKEOFF || movement == LANDING || movement == LANDED) return;
	AnimatorHolder::MarkAsSuspended(rightAnimator);
	AnimatorHolder::MarkAsSuspended(revrightAnimator);
	AnimatorHolder::MarkAsSuspended(revleftAnimator);
	leftAnimator->Start(this, leftAnimation);
	AnimatorHolder::MarkAsRunning(leftAnimator);
	movement = LEFT;
}

void Player::MoveRight(){
	if (movement == RIGHT || movement == LOOP || movement == TAKEOFF || movement == LANDING || movement == LANDED) return;
	AnimatorHolder::MarkAsSuspended(leftAnimator);
	AnimatorHolder::MarkAsSuspended(revleftAnimator);
	AnimatorHolder::MarkAsSuspended(revrightAnimator);
	rightAnimator->Start(this, rightAnimation);
	AnimatorHolder::MarkAsRunning(rightAnimator);
	movement = RIGHT;
}

void Player::TakeOff() {
	if (movement == TAKEOFF || movement == LANDING) return;

	for (PathEntry *e : takeoffList) {
		e->repetitions = 1;
	}
	takeoffList.front()->repetitions = 10;

	int diffToBottom = SCREEN_H - (y + 10) - frameBox.h * ScaleFactor;
	takeoffList.back()->repetitions = (diffToBottom / 10) - 5;

	takeoffAnimation->SetPath(std::list<PathEntry *>(takeoffList));
	takeoffAnimator->Start(this, takeoffAnimation);
	AnimatorHolder::MarkAsSuspended(rightAnimator);
	AnimatorHolder::MarkAsSuspended(revrightAnimator);
	AnimatorHolder::MarkAsSuspended(leftAnimator);
	AnimatorHolder::MarkAsSuspended(revleftAnimator);
	AnimatorHolder::MarkAsSuspended(loopAnimator);
	AnimatorHolder::MarkAsSuspended(landingAnimator);
	AnimatorHolder::MarkAsRunning(takeoffAnimator);
	movement = TAKEOFF;
}

void Player::Land() {
	if (movement == TAKEOFF || movement == LANDING || movement == LANDED) return;

	for (PathEntry *e : landingList) {
		e->repetitions = 1;
	}
	landingList.back()->repetitions = 15;
	
	PathEntry *first = landingList.front();
	first->dx = SCREEN_W / 2 - (x + frameBox.w / 2);
	first->dy = (2.5 * SCREEN_H / 4) - (y + frameBox.h / 2);

	int repetitions = max(abs(first->dx), abs(first->dy)) / 12;

	if (repetitions > 0){
		first->dx /= repetitions;
		first->dy /= repetitions;
	}
	first->repetitions = repetitions;

	landingAnimation->SetPath(std::list<PathEntry *>(landingList));
	landingAnimator->Start(this, landingAnimation);

	AnimatorHolder::MarkAsSuspended(rightAnimator);
	AnimatorHolder::MarkAsSuspended(revrightAnimator);
	AnimatorHolder::MarkAsSuspended(leftAnimator);
	AnimatorHolder::MarkAsSuspended(revleftAnimator);
	AnimatorHolder::MarkAsSuspended(loopAnimator);
	AnimatorHolder::MarkAsSuspended(takeoffAnimator);
	AnimatorHolder::MarkAsRunning(landingAnimator);
	movement = LANDING;

	SetSideFighters(false);
}

void Player::shoot() {
	if (!dead && movement != LOOP && movement != TAKEOFF && movement != LANDING && movement != LANDED) {
		if (PlayerBullet::FireBullets(getPos())){
			al_play_sample(shotSound, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			if (hasSideFighters) {
				for (SideFighter *s : sideFightersList) {
					s->shoot();
				}
			}
		}
	}
}

void Player::Loop(){
	if (movement == TAKEOFF || movement == LOOP || movement == LANDING || movement == LANDED || GameController::Get().getLoops() == 0) return;
	for (PathEntry *e : loopList){
		e->repetitions = 1;
	}
	loopAnimation->SetPath(std::list<PathEntry *>(loopList));
	loopAnimator->Start(this, loopAnimation);
	AnimatorHolder::MarkAsSuspended(rightAnimator);
	AnimatorHolder::MarkAsSuspended(revrightAnimator);
	AnimatorHolder::MarkAsSuspended(leftAnimator);
	AnimatorHolder::MarkAsSuspended(revleftAnimator);
	AnimatorHolder::MarkAsRunning(loopAnimator);
	movement = LOOP;
	GameController::Get().decLoops();
}

playermovement_t Player::GetMovement(){
	return movement;
}

void Player::Move(bool up, bool down, bool left, bool right){
	timestamp_t curr_timestamp = TIMESTAMP(tickCount);
	float _x = 0, _y = 0;

	if (movement == TAKEOFF || movement == LANDING || movement == LANDED) return;
	if (!(up || down || left || right)) return;

	float moveSpeed = ((up || down) && (right || left) ? (float) sqrt((pow(speed, 2)/2.0f)) : speed);

	if (up) _y = -moveSpeed;
	if (down) _y += moveSpeed;
	if (left) _x = -moveSpeed;
	if (right) _x += moveSpeed;

	if (x + _x < 0 || x + _x + frameBox.w*ScaleFactor > SCREEN_W){
		_x = 0;
	}
	if (y + _y + (movement == LOOP ? -60 : 0) < 0 || y + _y + frameBox.h*ScaleFactor + (movement == LOOP ? 60 : 0) > SCREEN_H){
		_y = 0;
	}

	if (curr_timestamp - last_timestamp > move_delay){
		Move(_x, _y);
		last_timestamp = curr_timestamp;
	}
}

void Player::Move(offset_t dx, offset_t dy) {
	x += dx;
	y += dy;
	if (hasSideFighters) {
		for (SideFighter *s : sideFightersList) {
			s->Move(dx, dy);
		}
	}
}

const Point Player::getPos() const {
	return Point(x, y);
}

void Player::Explode() {
	setDead(true);
	GameController::Get().decLives();
	RegularWave::Get().Reset();
	if (hasSideFighters) {
		for (SideFighter *s : sideFightersList) {
			s->Explode();
		}
	}
	new PlayerExplosion(x, y);
}

void Player::LostSideFighter(SideFighter *s) {
	sideFightersList.remove(s);
}

bool Player::isDead() {
	return dead;
}

void Player::setDead(bool val) {
	if (val) PlayerBullet::SetQuadBullets(false);
	dead = val;
	isVisible = !val;
	state = (dead ? DEAD : ALIVE);
}

void Player::CollisionResult(Sprite *s){
	switch (s->GetType()){
	case spritetype_t::ENEMY:
	case spritetype_t::ENEMY_BULLET:
		if (movement == LOOP || movement == TAKEOFF || movement == LANDING) return;
		Explode();
		break;
	}
}

void Player::AnimationFinish(void){
	LatelyDestroyable::Add(this);
}

void Player::SetSideFighters(bool val) {
	hasSideFighters = val;
	
	if (val) {
		AnimationFilm *sideFilm = AnimationFilmHolder::Get().GetFilm("sidefighter.sprite");

		sideFightersList.push_back(new SideFighter(-30, -30, -(10 + sideFilm->GetFrameBox(0).w * ScaleFactor), 3));
		sideFightersList.push_back(new SideFighter(SCREEN_W + 30, -30, +(12 + GetFrameBox(0).w * ScaleFactor), 3));
	}
	else {
		sideFightersList.clear();
	}
}

Player::~Player(){
	AnimatorHolder::MarkAsSuspended(leftAnimator);
	AnimatorHolder::Cancel(leftAnimator);
	delete leftAnimator;
	delete leftAnimation;
	AnimatorHolder::MarkAsSuspended(revleftAnimator);
	AnimatorHolder::Cancel(revleftAnimator);
	delete revleftAnimator;
	delete revleftAnimation;

	AnimatorHolder::MarkAsSuspended(rightAnimator);
	AnimatorHolder::Cancel(rightAnimator);
	delete rightAnimator;
	delete rightAnimation;
	AnimatorHolder::MarkAsSuspended(revrightAnimator);
	AnimatorHolder::Cancel(revrightAnimator);
	delete revrightAnimator;
	delete revrightAnimation;

	AnimatorHolder::MarkAsSuspended(loopAnimator);
	AnimatorHolder::Cancel(loopAnimator);
	delete loopAnimator;
	delete loopAnimation;

	AnimatorHolder::MarkAsSuspended(takeoffAnimator);
	AnimatorHolder::Cancel(takeoffAnimator);
	delete takeoffAnimator;
	delete takeoffAnimation;

	AnimatorHolder::MarkAsSuspended(landingAnimator);
	AnimatorHolder::Cancel(landingAnimator);
	delete landingAnimator;
	delete landingAnimation;
}