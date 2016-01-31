#include "AnimationFilmHolder.h"
#include "Sprite.h"

enum SELECT{
	PLAY, OPTIONS, CREDITS, SCORES, QUIT
};

class GameMenu{

private:
	SELECT selected;
	Sprite* menu;
	Sprite* pointer;
	Sprite* header;
	float pos_x;
	float pos_y;
	float point_x;
	float point_y;
	int positions[5];
	int currentPos = 0;

public:
	GameMenu(){
		pos_x = 200;
		pos_y = 400;
		point_x = 180;
		point_y = 400;

		positions[0] = 400;
		positions[1] = 440;
		positions[2] = 480;
		positions[3] = 520;
		positions[4] = 560;

		menu = new Sprite(pos_x, pos_y, AnimationFilmHolder::Get().GetFilm("game.menu"), spritetype_t::UI);
		//menu->currFilm->scaleFactor = SCALE / 2;
		pointer = new Sprite(pos_x, pos_y, AnimationFilmHolder::Get().GetFilm("menu.pointer"), spritetype_t::UI);
		//pointer->currFilm->scaleFactor = SCALE / 2;
		header = new Sprite(100, 100, AnimationFilmHolder::Get().GetFilm("menu.back"), spritetype_t::UI);
		//header->currFilm->scaleFactor = SCALE / 2;
	}

	void LeaveMenu(){
		menu->SetState(spritestate_t::WAIT);
		pointer->SetState(spritestate_t::WAIT);
		header->SetState(spritestate_t::WAIT);
	}

	void Update(){
		pointer->Move(point_x, point_y);
	}

	void MoveUp(){

		if (point_y == positions[PLAY]){
			point_y = positions[QUIT];
			currentPos = 4;
		}
		else{
			point_y = positions[currentPos - 1];
			currentPos--;
		}
	}

	void MoveDown(){

		if (point_y == positions[QUIT]){
			point_y = positions[PLAY];
			currentPos = 0;
		}
		else{
			point_y = positions[currentPos + 1];
			currentPos++;
		}
	}

	int GetSelected(){
		return currentPos;
	}

	void Draw(ALLEGRO_BITMAP *target){

		header->currFilm->DisplayFrame(target, Point(100, 100), header->GetFrame());

		menu->SetFrame(0);
		menu->currFilm->DisplayFrame(target, Point(pos_x + 20, pos_y), menu->GetFrame());

		menu->SetFrame(1);
		menu->currFilm->DisplayFrame(target, Point(pos_x + 20, pos_y + 40), menu->GetFrame());
		
		menu->SetFrame(2);
		menu->currFilm->DisplayFrame(target, Point(pos_x + 20, pos_y + 80), menu->GetFrame());
		
		menu->SetFrame(3);
		menu->currFilm->DisplayFrame(target, Point(pos_x + 20, pos_y + 120), menu->GetFrame());
		
		menu->SetFrame(4);
		menu->currFilm->DisplayFrame(target, Point(pos_x + 20, pos_y + 160), menu->GetFrame());

		pointer->currFilm->DisplayFrame(target, Point(point_x, point_y), pointer->GetFrame());
	}

	~GameMenu(){
		SpriteHolder::Get().Remove(menu);
		SpriteHolder::Get().Remove(pointer);
		SpriteHolder::Get().Remove(header);
		delete menu;
		delete pointer;
		delete header;
	}
};