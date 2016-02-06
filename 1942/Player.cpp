#include "Player.h"
#include "GameController.h"
#include "PlayerExplosion.h"

void Player::movementAnimatorCallback(Animator *a, void *c){
	LatelyDestroyable::Add(a);
}

void Player::reverseAnimatorCallback(Animator *a, void *c){
	LatelyDestroyable::Add(a);
	((Player *)c)->SetFrame(0);
}

void Player::tumbleAnimatorCallback(Animator *a, void *c){
	((Player *)c)->movement = NONE;
}

void Player::checkpointAnimatorCallback(Animator *a, void *c) {
	((Player *)c)->movement = NONE;
}

Player::Player(void) : Sprite(SCREEN_W / 2 - AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).w*ScaleFactor / 2, SCREEN_H / 2, AnimationFilmHolder::Get().GetFilm("player.sprite"), spritetype_t::PLAYER), movement(NONE) {

	this->SetFrame(22);
	checkPoint = false;

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
		tumbleList.push_back(pE);
	}

	pE = new PathEntry();
	pE->dx = 0;
	pE->dy = -10;
	pE->delay = 120;
	pE->repetitions = 1;
	pE->frame = 0;

	tumbleList.push_back(pE);

	tumbleAnimation = new MovingPathAnimation(std::list<PathEntry *>(tumbleList), 1337);
	tumbleAnimator = new MovingPathAnimator();
	tumbleAnimator->SetOnFinish(tumbleAnimatorCallback, this);

	PathEntry *onBoard = new PathEntry();

	onBoard->dx = 0;
	onBoard->dy = -10;
	onBoard->delay = 120;
	onBoard->repetitions = 10;
	onBoard->frame = 22;

	onboardList.push_back(onBoard);

	for (int i = 0; i < 15; i++) {
		onBoard = new PathEntry();
		onBoard->dx = 0;
		switch (i) {
		case 0:
		case 1:
		case 2:
		case 12:
		case 13:
		case 14:
			onBoard->dy = -10;
			break;
		case 3:
		case 11:
			onBoard->dy = 0;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			onBoard->dy = 10;
			break;
		}
		onBoard->delay = 120;
		onBoard->frame = i + 7;
		onBoard->repetitions = 1;
		onboardList.push_back(onBoard);
	}

	onBoard = new PathEntry();
	onBoard->dx = 0;
	onBoard->dy = +10;
	onBoard->delay = 120;
	onBoard->repetitions = 10;
	onBoard->frame = 0;

	onboardList.push_back(onBoard);

	checkPointAnimation = new MovingPathAnimation(std::list<PathEntry *>(onboardList), 15);
	checkPointAnimator = new MovingPathAnimator();
	checkPointAnimator->SetOnFinish(checkpointAnimatorCallback, this);

	AnimatorHolder::Register(leftAnimator);
	AnimatorHolder::Register(rightAnimator);
	AnimatorHolder::Register(revleftAnimator);
	AnimatorHolder::Register(revrightAnimator);
	AnimatorHolder::Register(tumbleAnimator);
	AnimatorHolder::Register(checkPointAnimator);

	last_timestamp = 0;

	Tumble();
}

void Player::StopMoving(){
	if (movement == NONE || movement == TUMBLE) return;
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
	if (movement == LEFT || movement == TUMBLE) return;
	AnimatorHolder::MarkAsSuspended(rightAnimator);
	AnimatorHolder::MarkAsSuspended(revrightAnimator);
	AnimatorHolder::MarkAsSuspended(revleftAnimator);
	leftAnimator->Start(this, leftAnimation);
	AnimatorHolder::MarkAsRunning(leftAnimator);
	movement = LEFT;
}

void Player::MoveRight(){
	if (movement == RIGHT || movement == TUMBLE) return;
	AnimatorHolder::MarkAsSuspended(leftAnimator);
	AnimatorHolder::MarkAsSuspended(revleftAnimator);
	AnimatorHolder::MarkAsSuspended(revrightAnimator);
	rightAnimator->Start(this, rightAnimation);
	AnimatorHolder::MarkAsRunning(rightAnimator);
	movement = RIGHT;
}

void Player::SetCheckPoint(bool check) {
	checkPoint = check;
}

void Player::CheckPointTumble() {
	if (!checkPoint) return;

	checkPoint = false;
	checkPointAnimation->SetPath(std::list<PathEntry *>(onboardList));
	checkPointAnimator->Start(this, checkPointAnimation);
	AnimatorHolder::MarkAsSuspended(rightAnimator);
	AnimatorHolder::MarkAsSuspended(revrightAnimator);
	AnimatorHolder::MarkAsSuspended(leftAnimator);
	AnimatorHolder::MarkAsSuspended(revleftAnimator);
	AnimatorHolder::MarkAsSuspended(tumbleAnimator);
	AnimatorHolder::MarkAsRunning(checkPointAnimator);
	movement = TUMBLE;

}

void Player::Tumble(){
	if (movement == TUMBLE || GameController::Get().getTumbles() == 0) return;
	for (PathEntry *e : tumbleList){
		e->repetitions = 1;
	}
	tumbleAnimation->SetPath(std::list<PathEntry *>(tumbleList));
	tumbleAnimator->Start(this, tumbleAnimation);
	AnimatorHolder::MarkAsSuspended(rightAnimator);
	AnimatorHolder::MarkAsSuspended(revrightAnimator);
	AnimatorHolder::MarkAsSuspended(leftAnimator);
	AnimatorHolder::MarkAsSuspended(revleftAnimator);
	AnimatorHolder::MarkAsRunning(tumbleAnimator);
	movement = TUMBLE;
	GameController::Get().decTumbles();
}

playermovement_t Player::GetMovement(){
	return movement;
}

void Player::Move(bool up, bool down, bool left, bool right){
	timestamp_t curr_timestamp = TIMESTAMP(tickCount);
	float _x = 0, _y = 0;

	if (!(up || down || left || right)) return;

	float moveSpeed = ((up || down) && (right || left) ? (float) sqrt((pow(speed, 2)/2.0f)) : speed);

	if (up) _y = -moveSpeed;
	if (down) _y += moveSpeed;
	if (left) _x = -moveSpeed;
	if (right) _x += moveSpeed;

	if (x + _x < 0 || x + _x + frameBox.w*ScaleFactor > SCREEN_W){
		_x = 0;
	}
	if (y + _y + (movement == TUMBLE ? -60 : 0) < 0 || y + _y + frameBox.h*ScaleFactor + (movement == TUMBLE ? 60 : 0) > SCREEN_H){
		_y = 0;
	}

	if (curr_timestamp - last_timestamp > move_delay){
		x += _x;
		y += _y;
		last_timestamp = curr_timestamp;
	}
}

const Point Player::getPos() const {
	return Point(x, y);
}

void Player::Explode() {
	new PlayerExplosion(x, y);
}

void Player::CollisionResult(Sprite *s){
	switch (s->GetType()){
	case spritetype_t::ENEMY:
	case spritetype_t::ENEMY_BULLET:
		if (movement == TUMBLE) return;
		state = spritestate_t::DEAD;
		isVisible = false;
		GameController::Get().decLives();
		Explode();
		break;
	}
}

void Player::AnimationFinish(void){
	LatelyDestroyable::Add(this);
}


Player::~Player(){
	AnimatorHolder::MarkAsSuspended(leftAnimator);
	AnimatorHolder::Cancel(leftAnimator);
	delete leftAnimator;
	delete leftAnimation;
	AnimatorHolder::MarkAsSuspended(rightAnimator);
	AnimatorHolder::Cancel(rightAnimator);
	delete rightAnimator;
	delete rightAnimation;
}