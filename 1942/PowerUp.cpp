#include "PowerUp.h"
#include "GameController.h"
PowerUp::PowerUp(float _dx, float _dy, powertype_t _power) : Sprite(_dx, _dy, AnimationFilmHolder::Get().GetFilm("powerup.sprite"), spritetype_t::POWER_UP)
{
	power = _power;
	//std::cout << "PowerUp with type " << power << " created." << std::endl;

	switch (power)
	{
		case QUAD_GUN:
			this->SetFrame(0);
			break;
		case ENEMY_CRASH:
			this->SetFrame(1);
			break;
		case SIDE_FIGHTERS:
			this->SetFrame(2);
			break;
		case EXTRA_LIFE:
			this->SetFrame(3);
			break;
		case NO_ENEMY_BULLETS:
			this->SetFrame(4);
			break;
		case EXTRA_LOOP:
			this->SetFrame(5);
			break;
		case THOUSAND_POINTS:
			this->SetFrame(6);
			break;
	}
}

void PowerUp::CollisionResult(Sprite *s){
	switch (s->GetType()){
	case spritetype_t::PLAYER:
		handlePowerUp();
		state = spritestate_t::DEAD;
		isVisible = false;
		break;
	}
}

void PowerUp::handlePowerUp() {
	switch (power)
	{
	case QUAD_GUN:
		//std::cout << "Quad gun pow!" << std::endl;
		break;
	case ENEMY_CRASH:
		//std::cout << "Enemy crash pow!" << std::endl;
		SpriteHolder::Get().DestroyEnemies();
		break;
	case SIDE_FIGHTERS:
		//std::cout << "Side fighters pow!" << std::endl;
		break;
	case EXTRA_LIFE:
		//std::cout << "Extra life pow!" << std::endl;
		GameController::Get().incLives();
		break;
	case NO_ENEMY_BULLETS:
		//std::cout << "No enemy bullets pow!" << std::endl;
		break;
	case EXTRA_LOOP:
		//std::cout << "Extra loop pow!" << std::endl;
		GameController::Get().incTumbles();
		break;
	case THOUSAND_POINTS:
		//std::cout << "+1000 score pow!" << std::endl;
		GameController::Get().incScore(1000);
		break;
	}
}

PowerUp::~PowerUp()
{
}
