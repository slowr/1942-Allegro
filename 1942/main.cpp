#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include "types.h"
#include "AnimationFilm.h"
#include "Sprite.h"
#include "FrameRangeAnimator.h"
#include "FrameRangeAnimation.h"
#include "BitmapLoader.h"
#include "MovingAnimator.h"
#include "AnimationFilmHolder.h"
#include "AnimatorHolder.h"
#include "PlayerBullet.h"
#include "Player.h"
#include "SpriteHolder.h"
#include "CollisionChecker.h"
#include "Enemy.h"
#include "LatelyDestroyable.h"
#include "GameMenu.h"
#include "Waves.h"

unsigned long tickCount = 0;

enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_P, KEY_SPACE, KEY_ENTER, KEY_ESCAPE
};

enum STATES{
	PAUSED, PLAYING, MENU, EXIT
};

timestamp_t getSystemTime(void){
	SYSTEMTIME time;
	GetSystemTime(&time);
	return (time.wSecond * 1000) + time.wMilliseconds;
}

int main(int argc, char **argv)
{
	srand(getSystemTime());

	std::vector<Enemy*> reds;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *scrollingBackgroundBitmap = NULL;

	BitmapLoader bitmapLoader;

	bool key[8] = { false, false, false, false, false, false, false, false };
	bool states[4] = { false, true , false, false };
	bool redraw = true;
	bool pause = false;
	bool doexit = false;

	int gameFps = 0;
	unsigned long oldTick = 0;
	float gameTime = 0;
	float y = 0;

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	al_init_image_addon();

	scrollingBackgroundBitmap = bitmapLoader.Load("resources/sample_terrain.bmp");
	if (!scrollingBackgroundBitmap) {
		fprintf(stderr, "failed to create background bitmap!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	float bgWidth = al_get_bitmap_width(scrollingBackgroundBitmap);
	float bgHeight = al_get_bitmap_height(scrollingBackgroundBitmap);
	float bgScaledWidth = SCREEN_W;											// scale to match screen width
	float bgScaleFactor = ((float)bgScaledWidth / (float)bgWidth);
	float bgScaledHeight = bgScaleFactor * bgHeight;

	//Waves::Get().CreateWaves("resources/waves_init.data");
	/* Load all animation films */
	AnimationFilmHolder::Get().Load("resources/deeznutz.data");
	/* Initialize the bullet sprite */
	PlayerBullet bullets[PlayerBullet::MAX_BULLETS];
	/* Initialize the player sprite */
	Player * player = new Player();
	/* Initialize an enemy */
	//Enemy * enemy = new Enemy(50,50, "green.jet", RED);
	for (int i = 0; i < 5; i++){
		reds.push_back(new Enemy(-i*30, 300, "red.plane", enemysubtype_t::RED));
	}
	/**********************/
	//GameMenu *menu = new GameMenu();


	fprintf(stderr, "Loaded scrolling background [%f, %f]\n",
		bgWidth, bgHeight);
	fprintf(stderr, "Should be scaled to [%f, %f]\n",
		bgScaledWidth, bgScaledHeight);

	//al_set_target_bitmap(scrollingBackgroundBitmap);
	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	while (!doexit)
	{
		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (pause) continue;
			tickCount++;

			if (key[KEY_UP]){}
			if (key[KEY_DOWN]){}
			if (key[KEY_LEFT]){}
			if (key[KEY_RIGHT]){}

			if (key[KEY_LEFT] && !key[KEY_RIGHT]){
				player->MoveLeft();
			} else if (key[KEY_RIGHT] && !key[KEY_LEFT]){
				player->MoveRight();
			} else if (!key[KEY_LEFT] && !key[KEY_RIGHT]){
				player->StopMoving();
			} else if (key[KEY_LEFT] && key[KEY_RIGHT]){
				player->StopMoving();
			}

			y += (BG_SCROLL_SPEED / FPS);
			redraw = true;

			CollisionChecker::Get().Check();
			//Waves::Get().SpawnWave(tickCount);
			if (states[PLAYING]) player->Move(key[KEY_UP], key[KEY_DOWN], key[KEY_LEFT], key[KEY_RIGHT], TIMESTAMP(tickCount));
			/*else if (states[MENU]){
				if (key[KEY_UP]) {
					menu->MoveUp();
					key[KEY_UP] = false;
				}

				if (key[KEY_DOWN]) {
					menu->MoveDown();
					key[KEY_DOWN] = false;
				}

				if (key[KEY_ENTER]){

				}

				if (key[KEY_ESCAPE]){
					doexit = true;
				}

				menu->Update();
			}*/

			if (bgHeight - (SCREEN_H / bgScaleFactor) - y <= 0) {
				// TODO: done scrolling!
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;

				break;

			case ALLEGRO_KEY_P:
				std::cout << "THIS" << tickCount << std::endl;
				pause = !pause;
				break;

			case ALLEGRO_KEY_ENTER:
				key[KEY_ENTER] = true;
				/*if (menu->GetSelected() == 0){
					states[PLAYING] = true; states[MENU] = false; menu->LeaveMenu(); y = 0;
					tickCount = 0;
				}*/
				break;

			case ALLEGRO_KEY_SPACE:
				PlayerBullet::FireBullets(bullets, player->getPos(), TIMESTAMP(tickCount));
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;

			case ALLEGRO_KEY_ENTER:
				key[KEY_ENTER] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_scaled_bitmap(scrollingBackgroundBitmap,
				0, bgHeight - (SCREEN_H / bgScaleFactor) - y, bgWidth, bgHeight,
				0, 0, bgScaledWidth, bgScaledHeight,
				0);

			/*if (states[MENU]){
				menu->Draw(al_get_backbuffer(display));
			}
			else*/ 
			if (states[PLAYING]){
				LatelyDestroyable::Destroy();
				AnimatorHolder::Progress(TIMESTAMP(tickCount));
				SpriteHolder::Get().DrawSprites(al_get_backbuffer(display));
			}

			al_flip_display();
		}
	}

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}