#include "PowerUp.h"


PowerUp::PowerUp(float _dx, float _dy, powertype_t _power) : Sprite(_dx, _dy, AnimationFilmHolder::Get().GetFilm("powerup.sprite"), spritetype_t::POWER_UP)
{
	power = _power;
	std::cout << "PowerUp with type " << power << " created." << std::endl;

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

void PowerUp::CollisionResult(spritetype_t type){
	switch (type){
	case spritetype_t::PLAYER:
		SpriteHolder::Get().SetPower(power);
		state = spritestate_t::DEAD;
		isVisible = false;
		break;
	}
}

PowerUp::~PowerUp()
{
}
