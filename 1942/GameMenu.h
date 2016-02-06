#include "AnimationFilmHolder.h"
#include "Sprite.h"
#include "SpriteHolder.h"
#include <vector>

class GameMenu {

private:
	std::vector<Sprite *> * Menu;
	float pos_x;
	float pos_y;
	int currentPos = 0;

public:
	GameMenu();
	void LeaveMenu();
	void Update();
	void MoveUp();
	void MoveDown();
	int GetSelected();
	void Draw();
	~GameMenu();
};