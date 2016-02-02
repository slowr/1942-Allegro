#include "Player.h"

void Player::movementAnimatorCallback(Animator *a, void *c){
	LatelyDestroyable::Add(a);
}

Player::Player(void) : Sprite(SCREEN_W / 2 - AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).w / 2, SCREEN_H - AnimationFilmHolder::Get().GetFilm("player.sprite")->GetFrameBox(0).h, AnimationFilmHolder::Get().GetFilm("player.sprite"), spritetype_t::PLAYER){
	
	leftAnimation = new FrameRangeAnimation(4, 6, 0, 0, delay, false, 1);
	leftAnimator = new FrameRangeAnimator();
	leftAnimator->SetOnFinish(movementAnimatorCallback, this);
	leftAnimator->Start(this, leftAnimation, TIMESTAMP(tickCount));

	rightAnimation = new FrameRangeAnimation(1, 3, 0, 0, delay, false, 1);
	rightAnimator = new FrameRangeAnimator();
	rightAnimator->SetOnFinish(movementAnimatorCallback, this);
	rightAnimator->Start(this, rightAnimation, TIMESTAMP(tickCount));

	//tumbleAnimation = new FrameRangeAnimation()

	AnimatorHolder::Register(leftAnimator);
	AnimatorHolder::Register(rightAnimator);
	last_timestamp = 0;
}

void Player::StopMoving(){
	SetFrame(0);
	movement = NONE;
	AnimatorHolder::MarkAsSuspended(leftAnimator);
	AnimatorHolder::MarkAsSuspended(rightAnimator);
}

void Player::MoveLeft(){
	if (movement == LEFT) return;
	AnimatorHolder::MarkAsSuspended(rightAnimator);
	leftAnimator->Start(this, leftAnimation, TIMESTAMP(tickCount));
	AnimatorHolder::MarkAsRunning(leftAnimator);
	movement = LEFT;
}

void Player::MoveRight(){
	if (movement == RIGHT) return;
	AnimatorHolder::MarkAsSuspended(leftAnimator);
	rightAnimator->Start(this, rightAnimation, TIMESTAMP(tickCount));
	AnimatorHolder::MarkAsRunning(rightAnimator);
	movement = RIGHT;
}

void Player::Tumble(){
	//TO DO
}

void Player::Move(bool up, bool down, bool left, bool right, timestamp_t curr_timestamp){
	int _x = 0, _y = 0;

	if (!(up || down || left || right)) return;

	if (up) _y = -speed;
	if (down) _y += speed;
	if (left) _x = -speed;
	if (right) _x += speed;

	if (x + _x < 0 || x + _x + frameBox.w > SCREEN_W) return;
	if (y + _y < 0 || y + _y + frameBox.h > SCREEN_H) return;

	if (curr_timestamp - last_timestamp > 25){
		x += _x;
		y += _y;
		last_timestamp = curr_timestamp;
	}
}

const Point Player::getPos() const {
	return Point(x, y);
}

void Player::CollisionResult(spritetype_t type){
	
	switch (type){
	case spritetype_t::ENEMY:
	case spritetype_t::ENEMY_BULLET:
		state = spritestate_t::DEAD;
		lifes--;
		isVisible = false;
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