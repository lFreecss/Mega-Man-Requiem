#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Entities.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Timer.h"
#include "Brofiler\Brofiler.h"


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
	scene_timer = new j1Timer();
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	bool ret = true;
	LOG("Loading Scene");

	scroll_speed = config.child("scroll").attribute("speed").as_float();
	scroll_limit = config.child("scroll").attribute("limit").as_uint();
	map_limit = config.child("map").attribute("limit").as_uint();
	rock_level.create(config.child("levels").child("first_level").attribute("name").as_string());
	jail_level.create(config.child("levels").child("second_level").attribute("name").as_string());
	time_fade_to_black = config.child("delay").attribute("delay_fade_to_black").as_float();
	
	LOG("Charging textures");
	title_bg_path.create(config.child("textures").child("bg").attribute("title").as_string());
	buttons_path.create(config.child("textures").child("ui").attribute("buttons").as_string());
	settings_bg_path.create(config.child("textures").child("bg").attribute("settings").as_string());
	settings_scrn_path.create(config.child("textures").child("ui").attribute("set_scrn").as_string());
	items_path.create(config.child("textures").child("ui").attribute("items").as_string());

	LOG("Charging all audio");
	music_title.create(config.child("music").child("menus").attribute("title").as_string());
	music_settings.create(config.child("music").child("menus").attribute("settings").as_string());
	music_1st_lvl.create(config.child("music").child("level").attribute("guts_man").as_string());
	music_2nd_lvl.create(config.child("music").child("level").attribute("cut_man").as_string());
	sound_landing.create(config.child("sounds").child("player").attribute("landing").as_string());
	sound_defeat.create(config.child("sounds").child("player").attribute("defeat").as_string());
	sound_start.create(config.child("sounds").child("buttons").attribute("game_start").as_string());
	sound_button_select.create(config.child("sounds").child("buttons").attribute("button_select").as_string());
	sound_game_over.create(config.child("sounds").child("player").attribute("game_over").as_string());
	sound_get_item.create(config.child("sounds").child("player").attribute("get_item").as_string());
	sound_victory.create(config.child("sounds").child("player").attribute("victory").as_string());
	sound_pause.create(config.child("sounds").child("buttons").attribute("pause").as_string());

	LOG("Charging all enemy positions");
	pugi::xml_node enemy_pos = config.child("enemy_pos");
	air_enem_1_1.x = enemy_pos.child("Air1-1").attribute("x").as_int();
	air_enem_1_1.y = enemy_pos.child("Air1-1").attribute("y").as_int();
	ground_enem_1_1.x = enemy_pos.child("Ground1-1").attribute("x").as_int();
	ground_enem_1_1.y = enemy_pos.child("Ground1-1").attribute("y").as_int();
	air_enem_1_2.x = enemy_pos.child("Air1-2").attribute("x").as_int();
	air_enem_1_2.y = enemy_pos.child("Air1-2").attribute("y").as_int();
	ground_enem_1_2.x = enemy_pos.child("Ground1-2").attribute("x").as_int();
	ground_enem_1_2.y = enemy_pos.child("Ground1-2").attribute("y").as_int();
	air_enem_2_1.x = enemy_pos.child("Air2-1").attribute("x").as_int();
	air_enem_2_1.y = enemy_pos.child("Air2-1").attribute("y").as_int();
	ground_enem_2_1.x = enemy_pos.child("Ground2-1").attribute("x").as_int();
	ground_enem_2_1.y = enemy_pos.child("Ground2-1").attribute("y").as_int();
	air_enem_2_2.x = enemy_pos.child("Air2-2").attribute("x").as_int();
	air_enem_2_2.y = enemy_pos.child("Air2-2").attribute("y").as_int();
	ground_enem_2_2.x = enemy_pos.child("Ground2-2").attribute("x").as_int();
	ground_enem_2_2.y = enemy_pos.child("Ground2-2").attribute("y").as_int();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->player->active = false;
	App->map->active = false;
	App->entities->active = false;
	App->collision->active = false;

	App->audio->LoadFx(sound_landing.GetString()); //1
	App->audio->LoadFx(sound_defeat.GetString()); //2
	App->audio->LoadFx(sound_start.GetString()); //3
	App->audio->LoadFx(sound_button_select.GetString()); //4
	App->audio->LoadFx(sound_game_over.GetString()); //5
	App->audio->LoadFx(sound_get_item.GetString()); //6
	App->audio->LoadFx(sound_victory.GetString()); //7
	App->audio->LoadFx(sound_pause.GetString()); //8

	title_bg = App->tex->Load(title_bg_path.GetString());
	buttons = App->tex->Load(buttons_path.GetString());
	settings_bg = App->tex->Load(settings_bg_path.GetString());
	settings_scrn = App->tex->Load(settings_scrn_path.GetString());
	items = App->tex->Load(items_path.GetString());
	

	StartScreen();

	return true;
}


void j1Scene::UIInteraction(UI* UI_elem, BUTTON_EVENTS UI_state)
{
	if (UI_elem->GetType() == BUTTON) {
		Button* bttn = (Button*)UI_elem;
		switch (UI_state)
		{
		case NONE:
			break;
		case MOUSE_ENTER:
			if(bttn != plus_volume_music && bttn != minus_volume_music && bttn != plus_volume_sound && bttn != minus_volume_sound)
			App->audio->PlayFx(4, 0);
			break;
		case MOUSE_INSIDE:
			bttn->ChangeToHoverImg();
			break;
		case MOUSE_LEAVE:
			bttn->ChangeToNormalImg();
			break;
		case LEFT_MOUSE_PRESS:
			bttn->ChangeToPressedImg();

			if (bttn == start_bttn) {
				App->audio->PlayFx(3, 0);
				StartPlaying();
			}

			else if (bttn == load_bttn) {
				App->audio->PlayFx(3, 0);
				StartPlaying();
				App->LoadGame();
			}

			else if (bttn == settings_bttn)
				SettingsScreen();

			else if (bttn == credits_bttn)
				CreditsScreen();

			else if (bttn == plus_volume_music) {
				volume_music++;
				if (volume_music >= 128)
					volume_music = 128;
				Mix_VolumeMusic(volume_music);
			}

			else if (bttn == minus_volume_music) {
				volume_music--;
				if (volume_music > 128) 
					volume_music = 0;
				Mix_VolumeMusic(volume_music);
			}

			else if (bttn == plus_volume_sound) {
				volume_sound++;
				if (volume_sound >= 128)
					volume_sound = 128;
				Mix_Volume(-1, volume_sound);
				App->audio->PlayFx(4, 0); //Button select sound
			}

			else if (bttn == minus_volume_sound) {
				volume_sound--;
				if (volume_sound > 128)
					volume_sound = 0;
				Mix_Volume(-1, volume_sound);
				App->audio->PlayFx(4, 0); //Button select sound
			}

			else if (bttn == back_bttn)
				StartScreen();
			
			else if (bttn == quit_bttn)
					quit_pressed = true;

			break;
		case RIGHT_MOUSE_PRESS:
			break;
		}
	}
}


// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	//Debug for the UI elements
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		App->gui->debug_print = !App->gui->debug_print;

	if (App->map->active == false)
		ManageMenusUI();

	else if (App->map->active) {
		CheckMap();
		App->map->Draw();
		DebugKeys();
		ManageStageUI();

		//Time Game Over
		if (scene_time <= 0.0f)
		{
			GameOverScreen();
		}

		//Pause Game
		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && pause != nullptr)
		{
			App->isPaused = !App->isPaused;

			if (App->isPaused) {
				if(!App->fade_to_black->IsFading())
					pause->ChangeText("PAUSE");
				scene_timer->Pause();
			}
			else {
				pause->ChangeText(" ");
				scene_timer->Continue();
			}
			App->audio->PlayFx(8, 0); //Pause sound
		}

		//Scroll
		if (App->player->pos.x <= App->map->data.tile_width*App->map->data.width - scroll_limit)
			App->render->camera.x = -(scroll_speed * App->player->pos.x);


		//When player gets at the end of the level, change to the next
		if (App->player->pos.x >= App->map->data.tile_width*App->map->data.width - map_limit) {
			App->collision->EraseCollider(App->player->collider);

			if (current_level == ROCK) {
				ChangeMaps(JAIL);
				MapStart();
			}

			else {
				//ChangeMaps(ROCK);
				//MapStart();
				EndScreen();
			}
		}

	}

	is_loading = false;

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || quit_pressed)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::StartScreen() {
	App->gui->CleanUp();
	plus_volume_music = nullptr;
	minus_volume_music = nullptr;
	plus_volume_sound = nullptr;
	minus_volume_sound = nullptr;
	back_bttn = nullptr;
	volume_music_text = nullptr;
	volume_sound_text = nullptr;
	pause = nullptr;

	//For stopping every sound (of game over or ending screen)
	Mix_HaltChannel(-1);

	App->audio->PlayMusic(music_title.GetString(), 0.0f);

	App->gui->CreateImage({ 0,0 }, { 0, 0, 427, 287 }, title_bg, false, this); //Title image
	start_bttn = App->gui->CreateButton({ 60,170 }, { 6, 10, 38, 7 }, { 49, 10, 38, 8 }, { 6, 10, 38, 7 }, buttons, false, this);
	load_bttn = App->gui->CreateButton({ 60,190 }, { 9, 28, 30, 7 }, { 50, 27, 30, 8 }, { 9, 28, 30, 7 }, buttons, false, this);
	settings_bttn = App->gui->CreateButton({ 60,210 }, { 7, 43, 60, 7 }, { 76, 43, 60, 8 }, { 7, 43, 60, 7 }, buttons, false, this);
	credits_bttn = App->gui->CreateButton({ 60,230 }, { 6, 84, 52, 7 }, { 65, 84, 52, 8 }, { 6, 84, 52, 7 }, buttons, false, this);
	quit_bttn = App->gui->CreateButton({ 60,250 }, { 7, 57, 28, 7 }, { 42, 57, 28, 8 }, { 7, 57, 28, 7 }, buttons, false, this);
}

void j1Scene::StartPlaying() {
	App->fade_to_black->FadeToBlack(App->scene->time_fade_to_black);
	App->gui->CleanUp();

	App->player->active = true;
	App->map->active = true;
	App->entities->active = true;
	App->collision->active = true;

	start_bttn = nullptr;
	start_bttn = nullptr;
	load_bttn = nullptr;
	settings_bttn = nullptr;
	credits_bttn = nullptr;
	quit_bttn = nullptr;
	back_bttn = nullptr;

	//Initializing map and positions
	first_map = rock_level;
	current_map = rock_level.GetString();
	App->map->CleanUp();
	App->map->Load(first_map.GetString());
	App->player->pos = App->player->startPos;
	InitializeMap();
	App->audio->PlayMusic(music_1st_lvl.GetString(), 0.0f);


	
	//gUI
	App->gui->CreateImage({ 5,255 }, { 4, 85, 19, 18 }, items, false, this); //Lives
	life_count = App->gui->CreateLabel({ 30,260 }, "X3", App->gui->GetFont(MEGA_MAN_10_SIZE_12), { 255,255,255,255 }, false, this); //Live count
	punctuation = App->gui->CreateLabel({ 170,10 }, "0000000", App->gui->GetFont(MEGA_MAN_10_SIZE_12), { 255,255,255,255 }, false, this); //Punctuation
	//time_game = App->gui->CreateLabel({ 400,10 }, "000", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 255,255,255,255 }, false, this); //Limited Time
	time_game = App->gui->CreateLabel({ 200,70 }, "000", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 255,255,255,255 }, false, this); //Limited Time
	pause = App->gui->CreateLabel({ 170,130 }, "     ", App->gui->GetFont(MEGA_MAN_10_SIZE_12), { 255,255,255,255 }, false, this); // PAUSE

	
	letter_M_1 = App->gui->CreateImage({ 132,30 }, { 5, 31, 16, 16 }, items, false, this); //M
	letter_E = App->gui->CreateImage({ 152,30 }, { 24, 31, 16, 16 }, items, false, this); //E
	letter_G = App->gui->CreateImage({ 172,30 }, { 43, 31, 16, 16 }, items, false, this); //G
	letter_A_1 = App->gui->CreateImage({ 192,30 }, { 62, 31, 16, 16 }, items, false, this); //A
	letter_M_2 = App->gui->CreateImage({ 212,30 }, { 5, 31, 16, 16 }, items, false, this); //M 2
	letter_A_2 = App->gui->CreateImage({ 232,30 }, { 62, 31, 16, 16 }, items, false, this); //A 2
	letter_N = App->gui->CreateImage({ 252,30 }, { 81, 31, 16, 16 }, items, false, this); //N
	letter_R = App->gui->CreateImage({ 272,30 }, { 100, 31, 16, 16 }, items, false, this); //R
	EnemySpawn();
}

void j1Scene::CreditsScreen() {
	App->gui->CleanUp();
	start_bttn = nullptr;
	load_bttn = nullptr;
	settings_bttn = nullptr;
	credits_bttn = nullptr;
	quit_bttn = nullptr;

	
	App->audio->PlayMusic(music_settings.GetString(), 0.0f);
	App->gui->CreateImage({ 0,0 }, { 0, 0, 427, 287 }, settings_bg, false, this); //Settings/Credits BG
	App->gui->CreateImage({ 30,40 }, { 0, 0, 368, 106 }, settings_scrn, false, this); //Settings/Credits black screen

	App->gui->CreateLabel({ 50, 20 }, "Licence:", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 255,255,255,255 }, false, this);
	App->gui->CreateLabel({ 40, 50 }, "MIT License Copyright(c)", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 0,255,0,255 }, false, this);
	App->gui->CreateLabel({ 40, 65 }, "Created by David Varela Hernandez and", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 0,255,0,255 }, false, this);
	App->gui->CreateLabel({ 40, 75 }, "David Valdivia Mart�nez", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 0,255,0,255 }, false, this);
	App->gui->CreateLabel({ 40, 95 }, "You can find more information about the", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 0,255,0,255 }, false, this);
	App->gui->CreateLabel({ 40, 105 }, "licence or about us in the README or in", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 0,255,0,255 }, false, this);
	App->gui->CreateLabel({ 40, 115 }, "our game Web site.", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 0,255,0,255 }, false, this);
	back_bttn = App->gui->CreateButton({ 10,250 }, { 7, 71, 31, 7 }, { 43, 71, 31, 8 }, { 7, 71, 31, 7 }, buttons, false, this);
}


void j1Scene::SettingsScreen() {
	App->gui->CleanUp();
	start_bttn = nullptr;
	load_bttn = nullptr;
	settings_bttn = nullptr;
	credits_bttn = nullptr;
	quit_bttn = nullptr;

	
	App->audio->PlayMusic(music_settings.GetString(), 0.0f);
	
	App->gui->CreateImage({ 0,0 }, { 0, 0, 427, 287 }, settings_bg, false, this); //Settings/Credits BG
	App->gui->CreateImage({ 110,60 }, { 0, 0, 184, 123 }, settings_scrn, false, this); //Settings/Credits black screen
	
	App->gui->CreateLabel({ 170, 20 }, "SETTINGS", App->gui->GetFont(MEGA_MAN_2), { 255,255,255,255 }, false, this);
	
	App->gui->CreateLabel({ 120, 70 }, "MUSIC VOLUME", App->gui->GetFont(MEGA_MAN_2), { 255,255,255,255 }, false, this);
	plus_volume_music = App->gui->CreateButton({ 150,95 }, { 164, 59, 16, 16 }, { 164, 59, 16, 16 }, { 205, 59, 16, 16 }, buttons, false, this); //Plus button
	volume_music_text = App->gui->CreateLabel({ 185, 97 }, (p2SString("%u", (volume_music))), App->gui->GetFont(MEGA_MAN_2), { 255,255,255,255 }, false, this);
	minus_volume_music = App->gui->CreateButton({ 230,95 }, { 183, 59, 16, 16 }, { 183, 59, 16, 16 }, { 224, 59, 16, 16 }, buttons, false, this); //Minus button

	App->gui->CreateLabel({ 120, 130 }, "SOUND VOLUME", App->gui->GetFont(MEGA_MAN_2), { 255,255,255,255 }, false, this);
	plus_volume_sound = App->gui->CreateButton({ 150,150 }, { 164, 59, 16, 16 }, { 164, 59, 16, 16 }, { 205, 59, 16, 16 }, buttons, false, this); //Plus button
	volume_sound_text = App->gui->CreateLabel({ 185, 150 }, (p2SString("%u", (volume_sound))), App->gui->GetFont(MEGA_MAN_2), { 255,255,255,255 }, false, this);
	minus_volume_sound = App->gui->CreateButton({ 230,150 }, { 183, 59, 16, 16 }, { 183, 59, 16, 16 }, { 224, 59, 16, 16 }, buttons, false, this); //Minus button

	back_bttn = App->gui->CreateButton({ 10,250 }, { 7, 71, 31, 7 }, { 43, 71, 31, 8 }, { 7, 71, 31, 7 }, buttons, false, this);
}

void j1Scene::GameOverScreen() {
	App->fade_to_black->FadeToBlack(App->scene->time_fade_to_black);
	
	App->gui->CleanUp();
	pause = nullptr;
	time_game = nullptr;
	App->player->active = false;
	App->map->active = false;
	App->entities->active = false;
	App->collision->active = false;
	
	App->entities->DeleteEnemy();
	App->entities->DeleteLetters();
	punctuation_count = 0;

	Mix_HaltMusic();
	App->audio->PlayFx(5, 0);

	App->gui->CreateImage({ 0,0 }, { 0, 0, 427, 287 }, settings_bg, false, this); //Settings/Credits BG
	App->gui->CreateImage({ 140,92 }, { 101, 68, 132, 70 }, items, false, this);
	App->gui->CreateLabel({ 150, 120 }, "GAME OVER", App->gui->GetFont(MEGA_MAN_10_SIZE_12), { 255,255,255,255 }, false, this);
	back_bttn = App->gui->CreateButton({ 180,180 }, { 6, 97, 48, 7 }, { 65, 97, 48, 8 }, { 6, 97, 48, 7 }, buttons, false, this);
}


void j1Scene::EndScreen() {
	//Disabiling and Cleaning up
	App->fade_to_black->FadeToBlack(App->scene->time_fade_to_black);
	pause = nullptr;
	App->gui->CleanUp();
	App->player->active = false;
	App->map->active = false;
	App->entities->active = false;
	App->collision->active = false;
	App->entities->DeleteEnemy();
	App->entities->DeleteLetters();
	
	Mix_HaltMusic();
	App->audio->PlayFx(7, 0); //Victory sound
	
	App->gui->CreateImage({ 0,0 }, { 0, 0, 427, 287 }, settings_bg, false, this); //Settings/Credits BG
	App->gui->CreateLabel({ 130, 30 }, "YOUR SCORE IS:", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 255,255,255,255 }, false, this);
	App->gui->CreateLabel({ 185, 50 }, (p2SString("%i", (punctuation_count))), App->gui->GetFont(MEGA_MAN_10_SIZE_12), { 255,255,255,255 }, false, this); //Punctuation
	App->gui->CreateImage({ 140, 92 }, { 101, 68, 132, 70 }, items, false, this); //Text rect
	App->gui->CreateLabel({ 165, 115 }, "THANKS FOR", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 255,255,255,255 }, false, this);
	App->gui->CreateLabel({ 175, 125 }, "PLAYING!", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 255,255,255,255 }, false, this);
	back_bttn = App->gui->CreateButton({ 180,180 }, { 6, 97, 48, 7 }, { 65, 97, 48, 8 }, { 6, 97, 48, 7 }, buttons, false, this);

	//Initializing variables for the next game loop
	punctuation_count = 0;
	App->player->lives = 3;
	App->player->Init();
}

void j1Scene::ManageStageUI() {
	life_count->ChangeText(p2SString("X%u", (App->player->GetLives())));


	scene_time = total_time_scene - scene_timer->ReadSec();
	if (!App->isPaused && time_game != nullptr)
	{
		if (scene_time >= 10.f)
		{
			time_game->ChangeText((p2SString("%i", ((int)scene_time))));
		}
		else {
			time_game->ChangeText((p2SString("%f", (scene_time))));
		}
	}

	if (punctuation_count >= 500) {
		punctuation->ChangeText((p2SString("0000%i", (punctuation_count))));
		letter_M_1->ChangeImage({ 135,31,16,16 });
		letter_E->ChangeImage({ 24, 31, 16, 16 }); //
		letter_G->ChangeImage({ 43, 31, 16, 16 }); //
		letter_A_1->ChangeImage({ 62, 31, 16, 16 }); //
		letter_M_2->ChangeImage({ 5, 31, 16, 16 }); //
		letter_A_2->ChangeImage({ 62, 31, 16, 16 }); //
		letter_N->ChangeImage({ 81, 31, 16, 16 }); //
		letter_R->ChangeImage({ 100, 31, 16, 16 }); //
	}

	if (punctuation_count > 500) {
		punctuation->ChangeText((p2SString("000%i", (punctuation_count))));
		if (punctuation_count >= 1000) {
			letter_E->ChangeImage({ 154,31,16,16 });
			letter_G->ChangeImage({ 43, 31, 16, 16 }); //
			letter_A_1->ChangeImage({ 62, 31, 16, 16 }); //
			letter_M_2->ChangeImage({ 5, 31, 16, 16 }); //
			letter_A_2->ChangeImage({ 62, 31, 16, 16 }); //
			letter_N->ChangeImage({ 81, 31, 16, 16 }); //
			letter_R->ChangeImage({ 100, 31, 16, 16 }); //
		}

		if (punctuation_count >= 1500) {
			letter_G->ChangeImage({ 173,31,16,16 });
			letter_A_1->ChangeImage({ 62, 31, 16, 16 }); //
			letter_M_2->ChangeImage({ 5, 31, 16, 16 }); //
			letter_A_2->ChangeImage({ 62, 31, 16, 16 }); //
			letter_N->ChangeImage({ 81, 31, 16, 16 }); //
			letter_R->ChangeImage({ 100, 31, 16, 16 }); //
		}

		if (punctuation_count >= 2000) {
			letter_A_1->ChangeImage({ 192,31,16,16 });
			letter_M_2->ChangeImage({ 5, 31, 16, 16 }); //
			letter_A_2->ChangeImage({ 62, 31, 16, 16 }); //
			letter_N->ChangeImage({ 81, 31, 16, 16 }); //
			letter_R->ChangeImage({ 100, 31, 16, 16 }); //
		}
	
		if (punctuation_count >= 2500) {
			letter_M_2->ChangeImage({ 135,31,16,16 });
			letter_A_2->ChangeImage({ 62, 31, 16, 16 }); //
			letter_N->ChangeImage({ 81, 31, 16, 16 }); //
			letter_R->ChangeImage({ 100, 31, 16, 16 }); //
		}
	
		if (punctuation_count >= 3000) {
			letter_A_2->ChangeImage({ 192,31,16,16 });
			letter_N->ChangeImage({ 81, 31, 16, 16 }); //
			letter_R->ChangeImage({ 100, 31, 16, 16 }); //
		}
		
		if (punctuation_count >= 3500) {
			letter_N->ChangeImage({ 211,31,16,16 });
			letter_R->ChangeImage({ 100, 31, 16, 16 }); //
		}

		if (punctuation_count >= 4000)
			letter_R->ChangeImage({ 230,31,16,16 });
	}
}

void j1Scene::ManageMenusUI() {

	if (volume_music_text != nullptr && volume_sound_text != nullptr) {
		if (volume_music > 99)
			volume_music_text->ChangeText(p2SString("%u", (volume_music)));
		else if (volume_music <= 99 && volume_music > 9)
			volume_music_text->ChangeText(p2SString("0%u", (volume_music)));
		else if (volume_music <= 9)
			volume_music_text->ChangeText(p2SString("00%u", (volume_music)));

		if (volume_sound > 99)
			volume_sound_text->ChangeText(p2SString("%u", (volume_sound)));
		else if (volume_sound <= 99 && volume_sound > 9)
			volume_sound_text->ChangeText(p2SString("0%u", (volume_sound)));
		else if (volume_sound <= 9)
			volume_sound_text->ChangeText(p2SString("00%u", (volume_sound)));
	}

}

void j1Scene::DebugKeys(){

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->fade_to_black->FadeToBlack(App->scene->time_fade_to_black);
		Restart();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->fade_to_black->FadeToBlack(App->scene->time_fade_to_black);
		MapStart();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) 
	{
		App->fade_to_black->FadeToBlack(App->scene->time_fade_to_black);
		App->LoadGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN){
		App->collision->view_collision = !App->collision->view_collision;
		App->map->view_collision = !App->map->view_collision;
	}
		
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		App->player->is_invincible = !App->player->is_invincible;

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		App->framerate_capped = !App->framerate_capped;
	}
	    
	//Change the level
	//if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) 
		//App->player->pos.x = App->map->data.tile_width*App->map->data.width - 30;

}

void j1Scene::CheckMap() {

	if (map_num == 0 && current_level != ROCK) {
		ChangeMaps(ROCK);
		EnemyInitialation();
	}
	if (map_num == 1 && current_level != JAIL) {
		ChangeMaps(JAIL);
		EnemyInitialation();
	}

}

void j1Scene::ChangeMaps(LEVEL_ID level_name) {
	App->fade_to_black->FadeToBlack(App->scene->time_fade_to_black);
	App->map->CleanUp();
	if (level_name == ROCK) {
		App->map->Load(rock_level.GetString());
		current_map = rock_level.GetString();
		App->audio->PlayMusic(music_1st_lvl.GetString(), 0.0f);
	}
	if (level_name == JAIL) {
		App->map->Load(jail_level.GetString());
		current_map = jail_level.GetString();
		App->audio->PlayMusic(music_2nd_lvl.GetString(), 0.0f);
	}
	InitializeMap();
}

void j1Scene::InitializeMap() {
	if (current_map == rock_level.GetString()) {
		current_level = ROCK;
		map_num = 0;
	}
	if (current_map == jail_level.GetString()) {
		current_level = JAIL;
		map_num = 1;
	}
	LetterInitialation();
	if (!is_loading) {
		scene_timer->Start();
	}
}

void j1Scene::Restart() {
	ChangeMaps(ROCK);
	MapStart();
	map_num = 0;
}

void j1Scene::MapStart() {
	App->render->camera.x = 0;
	App->render->camera.x = 0;
	App->player->Init();
	EnemyInitialation();
}

//Initialize enemies for each level
void j1Scene::EnemyInitialation() {

	if (current_level == ROCK) {
		EnemySpawn();
	}
	if (current_level == JAIL) {
		App->entities->DeleteEnemy();
		App->entities->AddEnemy(AIR, air_enem_2_1.x, air_enem_2_1.y);
		App->entities->AddEnemy(GROUND, ground_enem_2_1.x, ground_enem_2_1.y);
		App->entities->AddEnemy(AIR, air_enem_2_2.x, air_enem_2_2.y);
		App->entities->AddEnemy(GROUND, ground_enem_2_2.x, ground_enem_2_2.y);
	}

}

void j1Scene::LetterInitialation() {
	App->entities->DeleteLetters();
	if (current_level == ROCK) {
		if(punctuation_count < 500)
		App->entities->AddEnemy(LETTER, 424, 217);
		if (punctuation_count < 1000)
		App->entities->AddEnemy(LETTER, 1085, 150);
		if (punctuation_count < 1500)
		App->entities->AddEnemy(LETTER, 1325, 40);
		if (punctuation_count < 2000)
		App->entities->AddEnemy(LETTER, 1536, 240);
	}
	if (current_level == JAIL) {
		if (punctuation_count < 2500)
		App->entities->AddEnemy(LETTER, 292, 115);
		if (punctuation_count < 3000)
		App->entities->AddEnemy(LETTER, 700, 100);
		if (punctuation_count < 3500)
		App->entities->AddEnemy(LETTER, 1110, 200);
		if (punctuation_count < 4000)
		App->entities->AddEnemy(LETTER, 1920, 10);
	}

}

//Enemies for the first level
void j1Scene::EnemySpawn() {
	App->entities->DeleteEnemy();
	App->entities->AddEnemy(AIR, air_enem_1_1.x, air_enem_1_1.y);
	App->entities->AddEnemy(GROUND, ground_enem_1_1.x, ground_enem_1_1.y);
	App->entities->AddEnemy(AIR, air_enem_1_2.x, air_enem_1_2.y);
	App->entities->AddEnemy(GROUND, ground_enem_1_2.x, ground_enem_1_2.y);
}

//Load map
bool j1Scene::Load(pugi::xml_node& data)
{
	map_num = data.child("map").attribute("name").as_int();
	punctuation_count = data.child("score").attribute("num").as_int();
	float transcurred_seconds = total_time_scene - data.child("scene_time").attribute("num").as_float();
	scene_timer->SetLoadTime(transcurred_seconds - time_fade_to_black*2);

	is_loading = true;
	LetterInitialation(); 
	pause->ChangeText(" ");
	return true;
}

// Save map
bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node map = data.append_child("map");

	map.append_attribute("name") = map_num;

	pugi::xml_node score = data.append_child("score");

	score.append_attribute("num") = punctuation_count;
	
	pugi::xml_node time_left = data.append_child("scene_time");
	float time = scene_time;
	if (App->isPaused) {
		scene_timer->Continue();
		time = atof(time_game->GetText().GetString()); //atof converts char* to float
	}
	time_left.append_attribute("num") = time;

	return true;
}