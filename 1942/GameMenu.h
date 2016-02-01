#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "SpriteHolder.h"
#include <vector>

class GameMenu {

private:
	std::vector<Sprite *> * Menu;
	float pos_x;
	float pos_y;
	float point_x;
	float point_y;
	int positions[5];
	int currentPos = 0;

public:
	GameMenu();
	void LeaveMenu();
	void Update();
	void MoveUp();
	void MoveDown();
	int GetSelected();
	void Draw(ALLEGRO_BITMAP *target);
	~GameMenu();
};