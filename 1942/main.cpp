#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <string>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_audio.h>
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
#include "RegularWave.h"

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
	al_init_image_addon();
	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
	al_init_acodec_addon();

	std::cout << "Initialized allegro addons." << std::endl;

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *scrollingBackgroundBitmap = NULL;

	BitmapLoader bitmapLoader;

	bool key[9] = { false, false, false, false, false, false, false, false, false };
	bool redraw = true;
	bool pause = false;
	bool doexit = false;

	//gamestates_t state = gamestates_t::MENU;
	GameController::Get().setGameState(gamestates_t::MENU);

	int gameFps = 0;
	unsigned long oldTick = 0;
	float gameTime = 0;
	
	//float y = 0;
	GameController::Get().setBackgroundY(0);

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

	if (!al_install_audio()) {
		fprintf(stderr, "failed to initialize audio!\n");
		return -1;
	}

	if (!al_reserve_samples(1)) {
		fprintf(stderr, "failed to reserve samples!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

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

	ScaleFactor = bgScaleFactor * 0.8;

	GameController::Get().bgPositionArgs(bgHeight, bgScaleFactor);

	al_reserve_samples(4);
	ALLEGRO_SAMPLE *themesong = al_load_sample("resources/theme.ogg");
	ALLEGRO_SAMPLE_INSTANCE* themeInstance = al_create_sample_instance(themesong);
	al_set_sample_instance_playmode(themeInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(themeInstance, al_get_default_mixer());

	



	ALLEGRO_BITMAP *backBuffer = al_get_backbuffer(display);
	//Waves::Get().CreateWaves("resources/waves_init.data");
	/* Load all animation films */
	AnimationFilmHolder::Get().Load("resources/filmholder.data");
	Player * player;
	/**********************/
	GameMenu *menu = new GameMenu();

	GameController::Get().SetMenu(menu);

	fprintf(stderr, "Loaded scrolling background [%f, %f]\n",
		bgWidth, bgHeight);
	fprintf(stderr, "Should be scaled to [%f, %f]\n",
		bgScaledWidth, bgScaledHeight);

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
	
	al_init_primitives_addon();

	ALLEGRO_EVENT ev;

	while (!doexit)
	{
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (pause) continue;
			tickCount++;

			if (GameController::Get().getGameState() == gamestates_t::PLAYING){
				al_play_sample_instance(themeInstance);

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

				if (key[KEY_SPACE]) {
					if (GameController::Get().getGameState() == gamestates_t::PLAYING) {
						player->shoot();
					}
				}
			}

			if (GameController::Get().getGameState() == gamestates_t::PLAYING){
				if (!player->isDead() && player->GetMovement() != LANDED && player->GetMovement() != LANDING && player->GetMovement() != TAKEOFF) {
					RegularWave::Get().SpawnRegular();
				}

				/*if (GameController::Get().isCheckPoint()) {
					std::cout << "IN CHECKPOINT" << std::endl;
					GameController::Get().getPlayer()->Land();
				} else if (((int)GameController::Get().getBackgroundY() % 6000) == 0 && !player->isDead()) {
					
				}*/
			}

			if (GameController::Get().getGameState() == MENU || (GameController::Get().getGameState() == PLAYING && !player->isDead() && player->GetMovement() != LANDED)){
				GameController::Get().setBackgroundY(GameController::Get().getBackgroundY() + (BG_SCROLL_SPEED / FPS));
			}
			GameController::Get().setRedraw(true);

			if (GameController::Get().getGameState() == gamestates_t::PLAYING) {
				CollisionChecker::Get().Check();
				player->Move(key[KEY_UP], key[KEY_DOWN], key[KEY_LEFT], key[KEY_RIGHT]);
			}

			if (bgHeight - (SCREEN_H / bgScaleFactor) - GameController::Get().getBackgroundY() <= 0) {
				// TODO: done scrolling!
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				if (GameController::Get().getGameState() == gamestates_t::MENU) menu->MoveUp();
				else key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				if (GameController::Get().getGameState() == gamestates_t::MENU) menu->MoveDown();
				else key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;

			case ALLEGRO_KEY_P:
				if (GameController::Get().getGameState() == PLAYING) {
					pause = !pause;
					if (pause) {
						GameController::Get().DrawPaused();
						GameController::Get().setRedraw(false);
						continue;
					}
				}
				break;

			case ALLEGRO_KEY_ENTER:
				key[KEY_ENTER] = true;
				if (menu->GetSelected() == 0 && GameController::Get().getGameState() == gamestates_t::MENU) {
					GameController::Get().setGameState(gamestates_t::PLAYING);
					menu->LeaveMenu();
					tickCount = 0;
					PlayerBullet::InitBullets();
					player = new Player();
					GameController::Get().Reset();
					GameController::Get().SetPlayer(player);
					player->TakeOff();

					// checkpoints debug
					// GameController::Get().setBackgroundY(3300);
					// GameController::Get().setBackgroundY(7000);
				}
				break;

			case ALLEGRO_KEY_SPACE:
				key[KEY_SPACE] = true;
				break;

			case ALLEGRO_KEY_D:
				if (GameController::Get().getGameState() == PLAYING) player->Loop();
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

		if (GameController::Get().getRedraw() && al_is_event_queue_empty(event_queue)) {
			GameController::Get().setRedraw(false);

			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_scaled_bitmap(scrollingBackgroundBitmap,
				0, bgHeight - (SCREEN_H / bgScaleFactor) - GameController::Get().getBackgroundY(), bgWidth, bgHeight,
				0, 0, bgScaledWidth, bgScaledHeight,
				0);

			LatelyDestroyable::Destroy();

			if (GameController::Get().getGameState() == gamestates_t::PLAYING) {
				AnimatorHolder::Progress();
			}
			
			SpriteHolder::Get().DrawSprites();

			if (GameController::Get().getGameState() == gamestates_t::PLAYING){
				GameController::Get().DrawUI();

				if (player->isDead()) {
					GameController::Get().DeathScreen();
				}
				if (player->GetMovement() == LANDED) {
					GameController::Get().CheckPointScreen();
				}
			}

			al_flip_display();
		}
	}

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}