#include "GameMenu.h"

enum SELECT {
	PLAY, OPTIONS, CREDITS, SCORES, QUIT
};

GameMenu::GameMenu(){
	Menu = new std::vector<Sprite *>[7];
	pos_x = SCREEN_W / 2;
	pos_y = SCREEN_H / 2;
	currentPos = 0;

	AnimationFilm * film = AnimationFilmHolder::Get().GetFilm("game.menu_pointer");
	Menu->push_back(new Sprite(pos_x - (AnimationFilmHolder::Get().GetFilm("game.menu_playgame")->GetFrameBox(0).w / 1.65) * ScaleFactor, pos_y, film, spritetype_t::UI));

	film = AnimationFilmHolder::Get().GetFilm("game.menu_background");
	Menu->push_back(new Sprite(SCREEN_W / 2 - (film->GetFrameBox(0).w / 2) * ScaleFactor, (film->GetFrameBox(0).h / 2)*ScaleFactor, film, spritetype_t::UI));
	
	film = AnimationFilmHolder::Get().GetFilm("game.menu_playgame");
	Menu->push_back(new Sprite(pos_x - (film->GetFrameBox(0).w / 2) * ScaleFactor, pos_y, film, spritetype_t::UI));
	
	film = AnimationFilmHolder::Get().GetFilm("game.menu_options");
	pos_y += film->GetFrameBox(0).h * ScaleFactor;
	Menu->push_back(new Sprite(pos_x - (film->GetFrameBox(0).w / 2) * ScaleFactor, pos_y, film, spritetype_t::UI));
	
	film = AnimationFilmHolder::Get().GetFilm("game.menu_credits");
	pos_y += film->GetFrameBox(0).h  * ScaleFactor;
	Menu->push_back(new Sprite(pos_x - (film->GetFrameBox(0).w / 2) * ScaleFactor, pos_y, film, spritetype_t::UI));
	
	film = AnimationFilmHolder::Get().GetFilm("game.menu_scoring");
	pos_y += film->GetFrameBox(0).h  * ScaleFactor;
	Menu->push_back(new Sprite(pos_x - (film->GetFrameBox(0).w / 2) * ScaleFactor, pos_y, film, spritetype_t::UI));
	
	film = AnimationFilmHolder::Get().GetFilm("game.menu_quit");
	pos_y += film->GetFrameBox(0).h  * ScaleFactor;
	Menu->push_back(new Sprite(pos_x - (film->GetFrameBox(0).w / 2) * ScaleFactor, pos_y, film, spritetype_t::UI));
}

void GameMenu::LeaveMenu(){
	for (unsigned int i = 0; i < Menu->size(); i++)
		Menu->at(i)->SetState(spritestate_t::WAIT);
}

void GameMenu::ShowMenu() {
	for (unsigned int i = 0; i < Menu->size(); i++)
		Menu->at(i)->SetState(spritestate_t::ALIVE);
}

void GameMenu::MoveUp(){
	AnimationFilm * film = AnimationFilmHolder::Get().GetFilm("game.menu_pointer");
	if (currentPos == 0){
		Menu->at(0)->Move(0, film->GetFrameBox(0).h * ScaleFactor * 4);
		currentPos = 4;
	}
	else{
		currentPos--;
		Menu->at(0)->Move(0, -film->GetFrameBox(0).h * ScaleFactor);
	}

}

void GameMenu::MoveDown(){
	AnimationFilm * film = AnimationFilmHolder::Get().GetFilm("game.menu_pointer");
	if (currentPos == 4){
		Menu->at(0)->Move(0, -film->GetFrameBox(0).h * ScaleFactor * 4);
		currentPos = 0;
	}
	else{
		currentPos++;
		Menu->at(0)->Move(0, film->GetFrameBox(0).h * ScaleFactor);
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