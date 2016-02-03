#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
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
#include "PowWave.h"
#include "GameController.h"

unsigned long tickCount = 0;
float ScaleFactor = 1;

enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_P, KEY_SPACE, KEY_ENTER, KEY_ESCAPE, KEY_D
};

timestamp_t getSystemTime(void){
	SYSTEMTIME time;
	GetSystemTime(&time);
	return (time.wSecond * 1000) + time.wMilliseconds;
}

int main(int argc, char **argv)
{
	srand(getSystemTime());

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *scrollingBackgroundBitmap = NULL;

	BitmapLoader bitmapLoader;

	bool key[9] = { false, false, false, false, false, false, false, false, false };
	bool redraw = true;
	bool pause = false;
	bool doexit = false;

	gamestates_t state = gamestates_t::MENU;

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

	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon

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

	ScaleFactor = bgScaleFactor * 0.57;

	ALLEGRO_FONT *font = al_load_font("resources/BAUHS93.TTF",22,0);
	ALLEGRO_BITMAP *backBuffer = al_get_backbuffer(display);
	//Waves::Get().CreateWaves("resources/waves_init.data");
	/* Load all animation films */
	AnimationFilmHolder::Get().Load("resources/filmholder.data");
	/* Initialize the bullet sprite */
	PlayerBullet * bullets;
	Player * player;

	/**********************/
	GameMenu *menu = new GameMenu();


	fprintf(stderr, "Loaded scrolling background [%f, %f]\n",
		bgWidth, bgHeight);
	fprintf(stderr, "Should be scaled to [%f, %f]\n",
		bgScaledWidth, bgScaledHeight);

	//al_set_target_bitmap(scrollingBackgroundBitmap);
	al_set_target_bitmap(backBuffer);

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
	
	ALLEGRO_EVENT ev;

	while (!doexit)
	{
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (pause) continue;
			tickCount++;

			if (state == gamestates_t::PLAYING){
				if (key[KEY_LEFT] && !key[KEY_RIGHT]){
					player->MoveLeft();
				}
				else if (key[KEY_RIGHT] && !key[KEY_LEFT]){
					player->MoveRight();
				}
				else if (!key[KEY_LEFT] && !key[KEY_RIGHT]){
					player->StopMoving();
				}
				else if (key[KEY_LEFT] && key[KEY_RIGHT]){
					player->StopMoving();
				}

				if (key[KEY_SPACE]){
					if (state == gamestates_t::PLAYING && player->GetMovement() != TUMBLE) PlayerBullet::FireBullets(bullets, player->getPos(), TIMESTAMP(tickCount));
				}
			}

			if (state == gamestates_t::PLAYING){
				if (((int)y % 100) == 0){
					PowWave::Get().SpawnWave();
				}
			}

			y += (BG_SCROLL_SPEED / FPS);
			redraw = true;

			if (state == gamestates_t::PLAYING){
				CollisionChecker::Get().Check();
				player->Move(key[KEY_UP], key[KEY_DOWN], key[KEY_LEFT], key[KEY_RIGHT], TIMESTAMP(tickCount));
			}

			if (bgHeight - (SCREEN_H / bgScaleFactor) - y <= 0) {
				// TODO: done scrolling!
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				if (state == gamestates_t::MENU) menu->MoveUp();
				else key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				if (state == gamestates_t::MENU) menu->MoveDown();
				else key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;

			case ALLEGRO_KEY_P:
				pause = !pause;
				break;

			case ALLEGRO_KEY_ENTER:
				key[KEY_ENTER] = true;
				if (menu->GetSelected() == 0 && state == gamestates_t::MENU){
					state = gamestates_t::PLAYING;
					menu->LeaveMenu();
					y = 0;
					tickCount = 0;
					bullets = new PlayerBullet[MAX_BULLETS];
					player = new Player();
					GameController::Get().Reset();
					GameController::Get().SetPlayer(player);
				}
				break;

			case ALLEGRO_KEY_SPACE:
				key[KEY_SPACE] = true;
				break;

			case ALLEGRO_KEY_D:
				if (state == gamestates_t::PLAYING) player->Tumble();
				break;
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

			case ALLEGRO_KEY_SPACE:
				key[KEY_SPACE] = false;
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

			if (state == gamestates_t::PLAYING){
				al_draw_text(font, al_map_rgb(255, 255, 0), SCREEN_W / 2, 0, ALLEGRO_ALIGN_CENTRE, "HIGH SCORE");
				al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W * 0.1, 0, ALLEGRO_ALIGN_LEFT, "1UP");
				al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W * 0.9, 0, ALLEGRO_ALIGN_RIGHT, "2UP");
			}

			LatelyDestroyable::Destroy();
			if (state == gamestates_t::PLAYING) AnimatorHolder::Progress(TIMESTAMP(tickCount));
			SpriteHolder::Get().DrawSprites(backBuffer);

			al_flip_display();
		}
	}

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}