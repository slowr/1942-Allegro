#include "GameMenu.h"

enum SELECT {
	PLAY, OPTIONS, CREDITS, SCORES, QUIT
};

GameMenu::GameMenu(){
	Menu = new std::vector<Sprite *>[7];
	pos_x = 200;
	pos_y = 400;
	currentPos = 0;

	Menu->push_back(new Sprite(100, 100, AnimationFilmHolder::Get().GetFilm("game.menu_background"), spritetype_t::UI));
	Menu->push_back(new Sprite(pos_x - 20, pos_y, AnimationFilmHolder::Get().GetFilm("game.menu_pointer"), spritetype_t::UI));
	Menu->push_back(new Sprite(pos_x, pos_y, AnimationFilmHolder::Get().GetFilm("game.menu_playgame"), spritetype_t::UI));
	Menu->push_back(new Sprite(pos_x, pos_y + 20, AnimationFilmHolder::Get().GetFilm("game.menu_options"), spritetype_t::UI));
	Menu->push_back(new Sprite(pos_x, pos_y + 40, AnimationFilmHolder::Get().GetFilm("game.menu_credits"), spritetype_t::UI));
	Menu->push_back(new Sprite(pos_x, pos_y + 60, AnimationFilmHolder::Get().GetFilm("game.menu_scoring"), spritetype_t::UI));
	Menu->push_back(new Sprite(pos_x, pos_y + 80, AnimationFilmHolder::Get().GetFilm("game.menu_quit"), spritetype_t::UI));
}

void GameMenu::LeaveMenu(){
	for (unsigned int i = 0; i < Menu->size(); i++)
		Menu->at(i)->SetState(spritestate_t::WAIT);
}

void GameMenu::MoveUp(){
	if (currentPos == 0){
		Menu->at(1)->Move(0, 80);
		currentPos = 4;
	}
	else{
		currentPos--;
		Menu->at(1)->Move(0, -20);
	}

}

void GameMenu::MoveDown(){
	if (currentPos == 4){
		Menu->at(1)->Move(0, -80);
		currentPos = 0;
	}
	else{
		currentPos++;
		Menu->at(1)->Move(0, 20);
	}
}

int GameMenu::GetSelected(){
	return currentPos;
}

GameMenu::~GameMenu(){
	for (unsigned int i = 0; i < Menu->size(); i++)
		SpriteHolder::Get().Remove(Menu->at(i));
	delete Menu;
}