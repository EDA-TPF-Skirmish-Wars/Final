#include "./Graphics.h"
#include "../Network/Timer.h"
#include "./CSVParser/CSVParser.h"
#include "./Checksum/Checksum.h"

#define DISPLAY_WIDTH   1100
#define DISPLAY_HEIGHT  630
#define DISPLAY_WIDTH_OFFSET    15
#define DISPLAY_HEIGHT_OFFSET    15
#define TILE_SIDE   50
#define LEFT_CLICK  1
#define OPTIONS_MENU_AMOUNT  10
#define FONT_SIZE_SMALL		20
#define FONT_SIZE_LARGE		40

#define WIDTH_POPUP     100
#define POPUP_LINE      15  
#define HEIGHT_POPUP    (POPUP_LINE * OPTIONS_MENU_AMOUNT)


Graphics::Graphics() {
	al_init();
	this->graphicsError = G_NO_ERROR;
	//this->myMap = myMap;
	this->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	if (display == NULL) {
		graphicsError = G_DISPLAY_ERROR;
	}
	this->evQueue = al_create_event_queue();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();
	al_register_event_source(this->evQueue, al_get_keyboard_event_source());
	al_register_event_source(this->evQueue, al_get_mouse_event_source());
	al_register_event_source(this->evQueue, al_get_display_event_source(this->display));
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	if (!al_install_audio()) {
		graphicsError = G_AUDIO_ERROR;
	}
	if (!al_init_acodec_addon()) {
		graphicsError = G_AUDIO_ERROR;
	}
	voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	if (voice == NULL) {
		graphicsError = G_AUDIO_ERROR;
	}
	mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
	if (mixer == NULL) {
		graphicsError = G_AUDIO_ERROR;
	}
	if (!al_attach_mixer_to_voice(mixer, voice)) {
		graphicsError = G_AUDIO_ERROR;
	}
	sample = al_create_sample_instance(NULL);
	if (sample == NULL) {
		graphicsError = G_AUDIO_ERROR;
	}
	sample_data = al_load_sample("./Final/Graphics/resources/music.ogg");
	if (sample_data == NULL) {
		graphicsError = G_AUDIO_ERROR;
	}
	if (!al_set_sample(sample, sample_data)) {
		graphicsError = G_AUDIO_ERROR;
	}
	if (!al_attach_sample_instance_to_mixer(sample, mixer)) {
		graphicsError = G_AUDIO_ERROR;
	}
	al_set_sample_instance_playmode(sample, ALLEGRO_PLAYMODE_LOOP);
#ifdef PLAYMUSIC
	al_play_sample_instance(sample);
#endif // PLAYMUSIC

	font = al_load_font("./Final/Graphics/resources/font.ttf", FONT_SIZE_SMALL, 0); //VER SI ESTOS 2 CEROS ESTAN BIEN
	if (font == NULL) {
		graphicsError = G_LOAD_FONT_ERROR;
	}
	fontLarge = al_load_ttf_font("./Final/Graphics/resources/font.ttf", FONT_SIZE_LARGE, 0);
	if (fontLarge == NULL) {
		graphicsError = G_LOAD_FONT_ERROR;
	}
#ifdef SHOW_INTRODUCTION
	introduction();
#endif

	if (graphicsError == G_NO_ERROR) {
		setTeam();
	}
	/*if (graphicsError == G_NO_ERROR) {
		chooseMap();
	}
	if (graphicsError == G_NO_ERROR) {
		drawMap();
	}
	if (graphicsError == G_NO_ERROR) {
		al_flip_display();
	}*/

    return;
}

Graphics::~Graphics() {
	if (display != NULL) {
		al_destroy_display(display);
	}
	if (font != NULL) {
		al_destroy_font(font);
	}
	if (fontLarge != NULL) {
		al_destroy_font(fontLarge);
	}
	if (evQueue != NULL) {
		al_destroy_event_queue(evQueue);
	}
	al_set_sample(sample, NULL);
	al_destroy_sample(sample_data);
	al_destroy_sample_instance(sample);
	al_destroy_mixer(mixer);
	al_destroy_voice(voice);
	al_uninstall_audio();
	al_shutdown_ttf_addon();
	al_shutdown_font_addon();
	al_shutdown_image_addon();
	al_shutdown_primitives_addon();
    //disallocate memory
    return;
}

errors_s Graphics::updateGraphics(Map newMap){
	this->myMap = newMap;
	//showTransition();
	drawMap();
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 2, 0, "Wait for the Enemy!");
	if (graphicsError == G_NO_ERROR) {
		al_flip_display();
	}
    return graphicsError;
}

void Graphics::drawMap() {
	if (graphicsError == G_NO_ERROR) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		for (unsigned int u = 0; u < 12; u++) {
			showLine(u);
		}
		string str = "./Final/Graphics/resources/frame.png";
		ALLEGRO_BITMAP * bmp = al_load_bitmap(str.c_str());
		al_draw_scaled_bitmap(bmp, 67, 55, 995, 1490, 0, 0, TILE_SIDE * 16 + (DISPLAY_WIDTH_OFFSET * 2),
			TILE_SIDE * 12 + (DISPLAY_HEIGHT_OFFSET * 2), 0);
		al_destroy_bitmap(bmp);
		reDrawSide();
	}
	return;
}

void Graphics::showLine(unsigned int line) {
	//Dibuja los elementos que se encuentran en la linea i del mapa, las lineas van de 0-11 y las columnas de 0-15
	for (int i = 0; i < BOARD_WIDTH; i++) {
		Position pos(line, i);
		drawTerrain(myMap.getTerrain(pos), pos);
	}
	for (int i = 0; i < BOARD_WIDTH; i++) {
		Position pos(line, i);
		if (myMap.getBuildingPtr(pos) != nullptr) {
			drawBuilding(myMap.getBuilding(pos));
		}
	}
	for (int i = 0; i < BOARD_WIDTH; i++) {
		Position pos(line, i);
		if (myMap.getUnitPtr(pos) != nullptr) {
			drawUnit(myMap.getUnit(pos));
		}
	}
	return;
}

void Graphics::drawTerrain(terrains_d terrainToDraw, Position pos){
	if (graphicsError == G_NO_ERROR && terrainToDraw != BUILDING) {
#ifdef FOW
		if (myMap.getFog(pos) == false) {
#endif


			ALLEGRO_BITMAP * bmp = al_load_bitmap(getTerrainImagePath(terrainToDraw, pos).c_str());
			if (bmp != NULL) {
				al_draw_scaled_bitmap(bmp, 0, 0, 350,350, pos.column * TILE_SIDE + DISPLAY_WIDTH_OFFSET,
					pos.row * TILE_SIDE + DISPLAY_HEIGHT_OFFSET, TILE_SIDE, TILE_SIDE, 0);
				al_destroy_bitmap(bmp);
			}
			else
				graphicsError = G_LOAD_BITMAP_ERROR;
#ifdef FOW
		}
#endif
	}
	return;
}

void Graphics::drawBuilding(Building buildingToDraw){
	if (graphicsError == G_NO_ERROR) {
#ifdef FOW
		if (myMap.getFog(buildingToDraw.getPosition()) == false || buildingToDraw.getBuildingTeam() == myMap.getTeam()) {
#endif
			ALLEGRO_BITMAP * bmp = al_load_bitmap(getBuildingImagePath(buildingToDraw.getBuildingType(),
				buildingToDraw.getBuildingTeam()).c_str());
			if (bmp != NULL) {
				al_draw_scaled_bitmap(bmp, 0, 0, 350, 350, buildingToDraw.getPosition().column * TILE_SIDE + DISPLAY_WIDTH_OFFSET,
					buildingToDraw.getPosition().row * TILE_SIDE + DISPLAY_HEIGHT_OFFSET, TILE_SIDE, TILE_SIDE, 0);
				al_destroy_bitmap(bmp);
			}
			else
				graphicsError = G_LOAD_BITMAP_ERROR;
#ifdef FOW
		}
#endif
	}
	return; //error;
}

void Graphics::drawUnit(Unit unitToDraw){
	if (graphicsError == G_NO_ERROR) {
#ifdef FOW
		if (myMap.getFog(unitToDraw.getPosition()) == false || unitToDraw.getTeam() == myMap.getTeam()) {
#endif
			ALLEGRO_BITMAP * bmp = al_load_bitmap(getUnitImagePath(unitToDraw.getUnitClass(),
				unitToDraw.getTeam()).c_str());

			if (bmp != NULL) {
				al_draw_scaled_bitmap(bmp, 0,0,105,105, unitToDraw.getPosition().column * TILE_SIDE + DISPLAY_WIDTH_OFFSET, //51,58
					unitToDraw.getPosition().row * TILE_SIDE + DISPLAY_HEIGHT_OFFSET,TILE_SIDE,TILE_SIDE,0);
				al_destroy_bitmap(bmp);
			}
			else
				graphicsError = G_LOAD_BITMAP_ERROR;
#ifdef FOW
		}
#endif
	}
	return;// error;
}

action_s Graphics::getUserAction(int money){
	drawMap();
    action_s action;
	action.act = A_NO_ACTION;
	//drawMessage();
	action = getMouseAction(money);
	if (action.act == A_CLOSE_GAME) {
		al_destroy_display(display);
		display = NULL;
		graphicsError = G_GAME_CLOSED;
	}
    return action;
}

action_s Graphics::getMouseAction(int money){
    ALLEGRO_EVENT ev;
    action_s temp;
	do {
		drawMessage();
		bool tmp = true;
		if (graphicsError == G_NO_ERROR) {
			while (tmp) {
				al_flush_event_queue(this->evQueue);
				al_wait_for_event(this->evQueue, &ev);
				if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE || ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
					tmp = false;
				}
			}
			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				int x = ev.mouse.x;
				int y = ev.mouse.y;
				int xTile, yTile;
				xTile = (x - DISPLAY_WIDTH_OFFSET) / TILE_SIDE;
				yTile = (y - DISPLAY_HEIGHT_OFFSET) / TILE_SIDE;
				temp.act = A_NO_ACTION;
				Position pos(yTile, xTile);
				temp = showPopUp(myMap.getOptions(pos), xTile, yTile, money);
			}
			else
				temp.act = A_CLOSE_GAME;
		}
		else {
			temp.act = A_CLOSE_GAME;
		}
	} while (temp.act == A_BACK);
    return temp;
}

action_s Graphics::showPopUp(options_s opt, int xTile, int yTile, int money) {
	action_s temp;
	//temp.act = A_BACK;
	reDrawSide();
	int amountOfLines = 1;
	if (graphicsError == G_NO_ERROR) {
		string str = "Money $";
		str = str + std::to_string(money);
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 0.7, 0, str.c_str());
		if (opt.HP != -1) {
			string hola = "Life:  ";
			hola = hola + std::to_string(opt.HP);
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 19, TILE_SIDE * 0.7, 0, hola.c_str());
		}
		if (opt.MP != -1) {
			string hola = "MP:  ";
			hola = hola + std::to_string(opt.MP);
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 20.2, TILE_SIDE * 0.7, 0, hola.c_str());
		}
		if (opt.attackUpAvailable) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "'W' to attack Up!");
			amountOfLines++;
		}
		if (opt.attackDownAvailable) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "'S' to attack Down!");
			amountOfLines++;
		}
		if (opt.attackRightAvailable) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "'D' to attack Right!");
			amountOfLines++;
		}
		if (opt.attackLeftAvailable) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "'A' to attack Left!");
			amountOfLines++;
		}
		if (opt.buyAvailable) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "'B' to Buy!");
			amountOfLines++;
		}
		if (opt.moveUpAvailable) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "Up Arrow to Move Up!");
			amountOfLines++;
		}
		if (opt.moveDownAvailable) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "Down Arrow to Move Down!");
			amountOfLines++;
		}
		if (opt.moveLeftAvailable) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "Left Arrow to Move Left!");
			amountOfLines++;
		}
		if (opt.moveRightAvailable) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "Right Arrow to Move Right!");
			amountOfLines++;
		}
		if (opt.passAvailable) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "'P' to Pass!");
			amountOfLines++;
		}
		if (opt.canUPunload) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "'I' to unload up!");
			amountOfLines++;
		}
		if (opt.canDOWNunload) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "'K' to unload Down!");
			amountOfLines++;
		}
		if (opt.canLEFTunload) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "'J' to unload Left!");
			amountOfLines++;
		}
		if (opt.canRIGHTunload) {
			al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "'L' to unload Right!");
			amountOfLines++;
		}
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * (amountOfLines + 1), 0, "'X' to go Back!");
		amountOfLines++;
		al_flip_display();
	}
	return getKeyboardAction(xTile, yTile, opt);
}

action_s Graphics::getKeyboardAction(int xTile, int yTile, options_s opt){
    ALLEGRO_EVENT ev;
    action_s action;
    action.act = A_NO_ACTION;
    while(action.act == A_NO_ACTION){
        al_get_next_event(this->evQueue,&ev);
        switch(ev.type){
            case ALLEGRO_EVENT_KEY_DOWN:
                switch(ev.keyboard.keycode){
                    case ALLEGRO_KEY_W:
						if (opt.attackUpAvailable) {
							action.act = A_ATTACK;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile;
							action.positionTo.row = yTile - 1;
						}
                        break;
                    case ALLEGRO_KEY_A:
						if (opt.attackLeftAvailable) {
							action.act = A_ATTACK;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile - 1;
							action.positionTo.row = yTile;
						}
                        break;
                    case ALLEGRO_KEY_S:
						if (opt.attackDownAvailable) {
							action.act = A_ATTACK;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile;
							action.positionTo.row = yTile + 1;
						}
                        break;
                    case ALLEGRO_KEY_D:
						if (opt.attackRightAvailable) {
							action.act = A_ATTACK;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile + 1;
							action.positionTo.row = yTile;
						}
                        break;
                    case ALLEGRO_KEY_B:
						if (opt.buyAvailable) {
							action.act = A_PURCHASE;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
						}
                        break;
                    case ALLEGRO_KEY_P:
                        action.act = A_PASS;
                        break;
                    case ALLEGRO_KEY_UP:
						if (opt.moveUpAvailable) {
							action.act = A_MOVE;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile;
							action.positionTo.row = yTile - 1;
						}
                        break;
                    case ALLEGRO_KEY_DOWN:
						if (opt.moveDownAvailable) {
							action.act = A_MOVE;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile;
							action.positionTo.row = yTile + 1;
						}
                        break;
                    case ALLEGRO_KEY_LEFT:
						if (opt.moveLeftAvailable) {
							action.act = A_MOVE;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile - 1;
							action.positionTo.row = yTile;
						}
                        break;
                    case ALLEGRO_KEY_RIGHT:
						if (opt.moveRightAvailable) {
							action.act = A_MOVE;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile + 1;
							action.positionTo.row = yTile;
						}
                        break;
					case ALLEGRO_KEY_I:
						if (opt.canUPunload) {
							action.act = A_UNLOAD;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile;
							action.positionTo.row = yTile - 1;
						}
						break;
					case ALLEGRO_KEY_J:
						if (opt.canLEFTunload) {
							action.act = A_UNLOAD;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile - 1;
							action.positionTo.row = yTile;
						}
						break;
					case ALLEGRO_KEY_K:
						if (opt.canDOWNunload) {
							action.act = A_UNLOAD;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile;
							action.positionTo.row = yTile + 1;
						}
						break;
					case ALLEGRO_KEY_L:
						if (opt.canRIGHTunload) {
							action.act = A_UNLOAD;
							action.positionFrom.column = xTile;
							action.positionFrom.row = yTile;
							action.positionTo.column = xTile + 1;
							action.positionTo.row = yTile;
						}
						break;
					case ALLEGRO_KEY_X:
						action.act = A_BACK;
						break;
                    default:
                        action.act = A_NO_ACTION;
                        break;

                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                action.act = A_CLOSE_GAME;
                break;
            default:
                action.act = A_NO_ACTION;
                break;
        }
    }
    return action;
}

errors_s Graphics::getError() {
	return graphicsError;
}

void Graphics::showTransition() {
	if (graphicsError == G_NO_ERROR) {
		for (unsigned int i = 255; i > 0; i--) {
			al_clear_to_color(al_map_rgb(i, i, i));
			al_draw_text(fontLarge, al_map_rgb((255 - i), (255 - i), (255 - i)), TILE_SIDE * 3, TILE_SIDE * 5, 0,
				"Some changes have been happening during night!");
			al_flip_display();
			//timerMiliseconds(1);
		}
		//timerMiliseconds(1);
		for (unsigned int i = 0; i < 255; i++) {
			al_clear_to_color(al_map_rgb(i, i, i));
			al_draw_text(fontLarge, al_map_rgb((255-i),(255-i), (255-i)), TILE_SIDE * 3, TILE_SIDE*5, 0, 
				"Some changes have been happening during night!");
			al_flip_display();
			//timerMiliseconds(1);
		}
	}
	return;
}

void Graphics::drawMessage() {
	if (graphicsError == G_NO_ERROR) {
		reDrawSide();
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17 , TILE_SIDE * 2, 0, "Your Turn!");
		al_flip_display();
	}
	return;
}

void Graphics::displayActionInvalid() {
	if (graphicsError == G_NO_ERROR) {
		string str = "./resources/pop-up.png";
		ALLEGRO_BITMAP * bmp = al_load_bitmap(str.c_str());
		if (bmp == NULL) {
			graphicsError = G_LOAD_BITMAP_ERROR;
		}
		else {
			al_draw_scaled_bitmap(bmp, 0, 0, 575, 600, DISPLAY_WIDTH / 4, DISPLAY_HEIGHT / 4, TILE_SIDE * 5, TILE_SIDE * 5, 0);
			al_destroy_bitmap(bmp);
			al_draw_text(font, al_map_rgb(0, 0, 0), (TILE_SIDE * 7) - 10, TILE_SIDE * 5, 0, "Action Invalid!");
		}
	}
	if (graphicsError == G_NO_ERROR) {
		al_flip_display();
		timerMiliseconds(1000);
	}
	drawMap();
	if (graphicsError == G_NO_ERROR) {
		al_flip_display();
	}
	return;
}

void Graphics::showDices(int yours, int enemys) {
	if (graphicsError == G_NO_ERROR) {
		al_draw_text(fontLarge, al_map_rgb(255, 255, 255), TILE_SIDE * 5, TILE_SIDE, 0, "The Dices Were:");
		string str1, str2, str3, str;
		str1 = std::to_string(yours);
		str2 = std::to_string(enemys);
		str = str1 + "   " + str2;
		al_draw_text(fontLarge, al_map_rgb(255, 255, 255), TILE_SIDE * 7, TILE_SIDE * 2, 0, str.c_str());
		al_flip_display();
		timerMiliseconds(100);
		drawMap();
		if (graphicsError == G_NO_ERROR) {
			al_flip_display();
		}
	}
	return;
}

string Graphics::getUnitImagePath(int typeOfUnit, int team) {
	string answer;
	switch (typeOfUnit) {
	case ANTIAIR:
		answer = "./Final/Graphics/resources/Images/units/antiair_";
		break;
	case APC:
		answer = "./Final/Graphics/resources/Images/units/apc_";
		break;
	case ARTILLERY:
		answer = "./Final/Graphics/resources/Images/units/artillery_";
		break;
	case INFANTRY:
		answer = "./Final/Graphics/resources/Images/units/infantry_";
		break;
	case MECH:
		answer = "./Final/Graphics/resources/Images/units/mech_";
		break;
	case MEDTANK:
		answer = "./Final/Graphics/resources/Images/units/medtank_";
		break;
	case RECON:
		answer = "./Final/Graphics/resources/Images/units/recon_";
		break;
	case ROCKET:
		answer = "./Final/Graphics/resources/Images/units/rocket_";
		break;
	case TANK:
		answer = "./Final/Graphics/resources/Images/units/tank_";
		break;
	default:
		break;
	}
	switch (team) {
	case TEAM_RED:
		answer = answer + "1.png";
		break;
	case TEAM_BLUE:
		answer = answer + "2.png";
		break;
	case TEAM_GREEN:
		answer = answer + "3.png";
		break;
	case TEAM_YELLOW:
		answer = answer + "4.png";
		break;
	default:
		answer.clear();
		break;

	}
	return answer;
}

string Graphics::getBuildingImagePath(int typeOfBuild, int team) {
	string answer;
	switch (typeOfBuild) {
	case CITY:
		answer = "./Final/Graphics/resources/Images/building/city_";
		break;
	case FACTORY:
		answer = "./Final/Graphics/resources/Images/building/factory_";
		break;
	case HQ:
		answer = "./Final/Graphics/resources/Images/building/hq_";
		break;
	default:
		break;
	}
	switch (team) {
	case TEAM_RED:
		answer = answer + "1.png";
		break;
	case TEAM_BLUE:
		answer = answer + "2.png";
		break;
	case TEAM_GREEN:
		answer = answer + "3.png";
		break;
	case TEAM_YELLOW:
		answer = answer + "4.png";
		break;
	case NEUTRAL:
		answer = answer + "0.png";
		break;
	default:
		answer.clear();
		break;
	}
	return answer;
}

string Graphics::getTerrainImagePath(terrains_d terrain, Position pos) {
	string answer;
	if (terrain == FOREST) {
		answer = "./Final/Graphics/resources/Images/terrain/forest.png";
	}
	else if (terrain == HILL) {
		answer = "./Final/Graphics/resources/Images/terrain/hill.png";
	}
	else if (terrain == GRASS) {
		answer = "./Final/Graphics/resources/Images/terrain/plain.png";
	}
	else if (terrain == RIVER) { //RIO
		answer = "./Final/Graphics/resources/Images/terrain/";
		bool isThereOneUp, isThereOneDown, isThereOneLeft, isThereOneRight;
		isThereOneUp = false;
		isThereOneDown = false;
		isThereOneLeft = false;
		isThereOneRight = false;
		Position pos1(pos.row-1, pos.column);
		if (pos1.row >= 0 && myMap.getTerrain(pos1) == RIVER) {
			isThereOneUp = true;
		}
		Position pos2(pos.row + 1, pos.column);
		if ( pos2.row <= 11 && myMap.getTerrain(pos2) == RIVER) {
			isThereOneDown = true;
		}
		Position pos3(pos.row, pos.column - 1);
		if ( pos3.column >= 0 && myMap.getTerrain(pos3) == RIVER) {
			isThereOneLeft = true;
		}
		Position pos4(pos.row, pos.column + 1);
		if ( pos4.column <= 15 && myMap.getTerrain(pos4) == RIVER) {
			isThereOneRight = true;
		}
		answer = answer + "r_";
		string str;
		if (isThereOneUp) {
			str = "u";
		}
		if (isThereOneLeft) {
			str = str + "l";
		}
		if (isThereOneRight) {
			str = str + "r";
		}
		if (isThereOneDown) {
			str = str + "d";
		}
		str = str + ".png";
		answer = answer + str;
	}
	else if (terrain == ROAD) { //Camino
		answer = "./Final/Graphics/resources/Images/terrain/";
		bool isThereOneUp, isThereOneDown, isThereOneLeft, isThereOneRight;
		isThereOneUp = false;
		isThereOneDown = false;
		isThereOneLeft = false;
		isThereOneRight = false;
		Position pos1(pos.row - 1, pos.column);
		if ( pos1.row >= 0 && myMap.getTerrain(pos1) == ROAD) {
			isThereOneUp = true;
		}
		Position pos2(pos.row + 1, pos.column);
		if ( pos2.row <= 11 && myMap.getTerrain(pos2) == ROAD) {
			isThereOneDown = true;
		}
		Position pos3(pos.row, pos.column - 1);
		if ( pos3.column >= 0 && myMap.getTerrain(pos3) == ROAD) {
			isThereOneLeft = true;
		}
		Position pos4(pos.row, pos.column + 1);
		if ( pos4.column <= 15 && myMap.getTerrain(pos4) == ROAD) {
			isThereOneRight = true;
		}
		answer = answer + "c_";
		string str;
		if (isThereOneUp) {
			str = "u";
		}
		if (isThereOneLeft) {
			str = str + "l";
		}
		if (isThereOneRight) {
			str = str + "r";
		}
		if (isThereOneDown) {
			str = str + "d";
		}
		str = str + ".png";
		answer = answer + str;
	}
	return answer;
}

void Graphics::reDrawSide() {
	string str = "./Final/Graphics/resources/pop-up.png";
	ALLEGRO_BITMAP *bmp = al_load_bitmap(str.c_str());
	al_draw_scaled_bitmap(bmp, 0, 0, 575, 600, TILE_SIDE * 16, -24, TILE_SIDE * 6.6, TILE_SIDE *13.5, 0);
	string top;
	top = myName;
	switch (myMap.getTeam()) {
	case TEAM_BLUE:
		top = top + "  Team Blue.";
		break;
	case TEAM_RED:
		top = top + "  Team Red.";
		break;
	case TEAM_GREEN:
		top = top + "  Team Green.";
		break;
	case TEAM_YELLOW:
		top = top + "  Team Yellow.";
		break;
	}
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 0, 0, top.c_str());
	top.clear();
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 11.5, 0, enemyName.c_str());
	al_destroy_bitmap(bmp);
	return;
}

void Graphics::introduction() {
	if (graphicsError == G_NO_ERROR) {
		ALLEGRO_BITMAP * bmp1 = al_load_bitmap("./Final/Graphics/resources/banner.png");
		if (bmp1 != NULL) {
			//timerMiliseconds(1000);
			for (int i = DISPLAY_HEIGHT; i > 0; i--) {
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_scaled_bitmap(bmp1, 0, 0, 1000, 500, 0, i, DISPLAY_WIDTH,DISPLAY_HEIGHT +i, 0);
				if (graphicsError == G_NO_ERROR) {
					al_flip_display();
				}
				timerMiliseconds(10);
			}
			//al_destroy_bitmap(bmp);
		}
		else {
			graphicsError = G_LOAD_BITMAP_ERROR;
		}
		ALLEGRO_BITMAP * bmp = al_load_bitmap("./Final/Graphics/resources/pressstart.png");
		if (bmp != NULL && graphicsError == G_NO_ERROR) {
			al_draw_bitmap(bmp, TILE_SIDE * 5, TILE_SIDE * 8,0);
			if (graphicsError == G_NO_ERROR) {
				al_flip_display();
			}
			//al_destroy_bitmap(bmp);
		}
		else {
			graphicsError = G_LOAD_BITMAP_ERROR;
		}
		ALLEGRO_EVENT ev;
		bool tmp = false;
		bool tmp2 = true;
		timerNB(1000);
		while (!tmp) {
			if (isTimerFinished()) {
				timerNB(1000);
				if (tmp2) {
					al_clear_to_color(al_map_rgb(255, 255, 255));
					al_draw_scaled_bitmap(bmp1, 0, 0, 1000, 500, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
					al_draw_bitmap(bmp, TILE_SIDE * 5, TILE_SIDE * 8, 0);
					al_flip_display();

				}
				else {
					al_clear_to_color(al_map_rgb(255, 255, 255));
					al_draw_scaled_bitmap(bmp1, 0, 0, 1000, 500, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
					al_flip_display();
				}
				tmp2 = !tmp2;
			}
			al_wait_for_event(evQueue, &ev);
			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN || ev.type == ALLEGRO_EVENT_KEY_DOWN
				|| ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				tmp = true;
			}
		}
		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 9, TILE_SIDE * 5, 0,"Please Wait");
		al_flip_display();
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			graphicsError = G_GAME_CLOSED;
		}
		al_destroy_bitmap(bmp1);
		al_destroy_bitmap(bmp);
	}
	return;
}

string Graphics::chooseMap() {
	string str = dispChoose();
	selectMap(str, 0,true);
	return str;
}

string Graphics::dispChoose() {
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 4,TILE_SIDE * 5, 0, "Press a number from 0 to 9 to choose a Map!");
	al_flip_display();
	ALLEGRO_EVENT ev;
	string str;
	bool tmp = true;
	while (tmp) {
		al_wait_for_event(evQueue, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			tmp = false;
			graphicsError = G_GAME_CLOSED;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_1:
				str = "./Final/Graphics/resources/maps/BalancedArena.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_2:
				str = "./Final/Graphics/resources/maps/BalancedRing.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_3:
				str = "./Final/Graphics/resources/maps/BlancedCross.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_4:
				str = "./Final/Graphics/resources/maps/IslandWar.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_5:
				str = "./Final/Graphics/resources/maps/MystPi.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_6:
				str = "./Final/Graphics/resources/maps/Nascar.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_7:
				str = "./Final/Graphics/resources/maps/SanFranciscoBridge.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_8:
				str = "./Final/Graphics/resources/maps/SnakeArena.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_9:
				str = "./Final/Graphics/resources/maps/SuperS.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_0:
				str = "./Final/Graphics/resources/maps/WaterWorld.csv";
				tmp = false;
				break;
			}
		}
	}
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 9, TILE_SIDE * 5, 0, "Please Wait!");
	al_flip_display();
	return str;

}

void Graphics::setTeam() {
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 6, TILE_SIDE * 4, 0, "Choose a color for your team!");
	al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 3, TILE_SIDE * 5, 0, "B for Blue, R for Red, G for green and Y for yellow");
	al_flip_display();
	ALLEGRO_EVENT ev;
	bool tmp = true;
	while (tmp) {
		al_wait_for_event(evQueue, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			tmp = false;
			graphicsError = G_GAME_CLOSED;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_B:
				myMap.setTeam(TEAM_BLUE);
				myMap.setEnemyTeam(TEAM_RED);
				tmp = false;
				break;
			case ALLEGRO_KEY_R:
				myMap.setTeam(TEAM_RED);
				myMap.setEnemyTeam(TEAM_BLUE);
				tmp = false;
				break;
			case ALLEGRO_KEY_G:
				myMap.setTeam(TEAM_GREEN);
				myMap.setEnemyTeam(TEAM_YELLOW);
				tmp = false;
				break;
			case ALLEGRO_KEY_Y:
				myMap.setTeam(TEAM_YELLOW);
				myMap.setEnemyTeam(TEAM_GREEN);
				tmp = false;
				break;
			}
		}
	}
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 9, TILE_SIDE * 5, 0, "Please Wait");
	al_flip_display();
	return;
}

string Graphics::getName() {
	string name;
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 4, TILE_SIDE * 5, 0, "Please enter your name and press Enter!");
	al_flip_display();
	ALLEGRO_EVENT ev;
	bool exit = false;
	int x, y;
	x = TILE_SIDE * 10;
	y = TILE_SIDE * 5;
	while (!exit) {
		al_get_next_event(this->evQueue, &ev);
		switch (ev.type) {
		case ALLEGRO_EVENT_KEY_DOWN:
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_A:
				name.push_back('a');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_B:
				name.push_back('b');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_C:
				name.push_back('c');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0),x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_D:
				name.push_back('d');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_E:
				name.push_back('e');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_F:
				name.push_back('f');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_G:
				name.push_back('g');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_H:
				name.push_back('h');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_I:
				name.push_back('i');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_J:
				name.push_back('j');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_K:
				name.push_back('k');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_L:
				name.push_back('l');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_M:
				name.push_back('m');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_N:
				name.push_back('n');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_O:
				name.push_back('o');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_P:
				name.push_back('p');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_Q:
				name.push_back('q');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_R:
				name.push_back('r');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_S:
				name.push_back('s');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_T:
				name.push_back('t');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_U:
				name.push_back('u');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_V:
				name.push_back('v');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_W:
				name.push_back('w');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_X:
				name.push_back('x');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_Y:
				name.push_back('y');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_Z:
				name.push_back('z');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_ENTER:
				exit = true;
				break;
			case ALLEGRO_KEY_BACKSPACE:
				name.pop_back();
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			case ALLEGRO_KEY_SPACE:
				name.push_back(' ');
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(fontLarge, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
				al_flip_display();
				break;
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			graphicsError = G_GAME_CLOSED;
			exit = true;
			break;
		default:
			break;
		}
	}
	myName = name;
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 9, TILE_SIDE * 5, 0, "Please Wait");
	al_flip_display();
	return name;
}

void Graphics::setEnemyName(string enemyName) {
	this->enemyName = enemyName;
	return;
}

void Graphics::selectMap(string mapName, int checksum, bool iCreateMap) {
	std::ifstream file;
	file.open(mapName);
	if (!file.is_open())
		graphicsError = G_LOAD_FILE_ERROR;
	Checksum chk(&file);
	this->checksum = chk.getChecksum();
	if (!iCreateMap && this->checksum != checksum) {
		graphicsError = G_WRONG_CHECKSUM;
	}
	file.close();
	if (graphicsError == G_NO_ERROR) {
		io::CSVReader<16> in(mapName);//"./resources/maps/BalancedArena.csv");
								  //in.read_header(io::ignore_extra_column, "vendor", "size", "speed");
		std::string col0, col1, col2, col3, col4, col5, col6, col7, col8, col9, col10, col11, col12, col13, col14, col15;
		int i = 0;
		while (in.read_row(col0, col1, col2, col3, col4, col5, col6, col7, col8, col9, col10, col11, col12, col13, col14, col15)) {
			int u = 0;
			string a;
			for (u = 0; u < 16; u++) {
				switch (u) {
				case 0:
					a = col0;
					break;
				case 1:
					a = col1;
					break;
				case 2:
					a = col2;
					break;
				case 3:
					a = col3;
					break;
				case 4:
					a = col4;
					break;
				case 5:
					a = col5;
					break;
				case 6:
					a = col6;
					break;
				case 7:
					a = col7;
					break;
				case 8:
					a = col8;
					break;
				case 9:
					a = col9;
					break;
				case 10:
					a = col10;
					break;
				case 11:
					a = col11;
					break;
				case 12:
					a = col12;
					break;
				case 13:
					a = col13;
					break;
				case 14:
					a = col14;
					break;
				case 15:
					a = col15;
					break;
				default:
					break;
				}
				Position pos(i, u);
				if (a.length() == 1) {
					if (a == "t") {
						myMap.addTile(pos, GRASS, false);
					}
					else if (a == "r") {
						myMap.addTile(pos, RIVER, false);
					}
					else if (a == "a") {
						myMap.addTile(pos, ROAD, false);
					}
					else if (a == "f") {
						myMap.addTile(pos, FOREST, false);
					}
					else if (a == "h") {
						myMap.addTile(pos, HILL, false);
					}
				}
				else if (a.length() == 2) {
					if (a == "m0") {
						myMap.addTile(pos, BUILDING, false);
						myMap.addBuilding(FACTORY, NEUTRAL, pos);
					}
					else if (a == "m1") {
						myMap.addTile(pos, BUILDING, false);
						if (iCreateMap) {
							myMap.addBuilding(FACTORY, myMap.getTeam(), pos);
						}
						else {
							myMap.addBuilding(FACTORY, myMap.getEnemyTeam(), pos);
						}
					}
					else if (a == "m2") {
						myMap.addTile(pos, BUILDING, false);
						if (iCreateMap) {
							myMap.addBuilding(FACTORY, myMap.getEnemyTeam(), pos);
						}
						else {
							myMap.addBuilding(FACTORY, myMap.getTeam(), pos);
						}
					}
					else if (a == "c0") {
						myMap.addTile(pos, BUILDING, false);
						myMap.addBuilding(CITY, NEUTRAL, pos);
					}
					else if (a == "c1") {
						myMap.addTile(pos, BUILDING, false);
						if (iCreateMap) {
							myMap.addBuilding(CITY, myMap.getTeam(), pos);
						}
						else {
							myMap.addBuilding(CITY, myMap.getEnemyTeam(), pos);
						}
					}
					else if (a == "c2") {
						myMap.addTile(pos, BUILDING, false);
						if (iCreateMap) {
							myMap.addBuilding(CITY, myMap.getEnemyTeam(), pos);
						}
						else {
							myMap.addBuilding(CITY, myMap.getTeam(), pos);
						}
					}
					else if (a == "q1") {
						myMap.addTile(pos, BUILDING, false);
						if (iCreateMap) {
							myMap.addBuilding(HQ, myMap.getTeam(), pos);
						}
						else {
							myMap.addBuilding(HQ, myMap.getEnemyTeam(), pos);
						}
					}
					else if (a == "q2") {
						myMap.addTile(pos, BUILDING, false);
						if (iCreateMap) {
							myMap.addBuilding(HQ, myMap.getEnemyTeam(), pos);
						}
						else {
							myMap.addBuilding(HQ, myMap.getTeam(), pos);
						}
					}
				}
				else if (a.length() == 5 && a[1] == '+') {
					units_d tmp;
					teams_d tmp2;
					unsigned int tmp3 = 0;
					if (a[0] == 't') {
						myMap.addTile(pos, GRASS, false);
					}
					else if (a[0] == 'r') {
						myMap.addTile(pos, RIVER, false);
					}
					else if (a[0] == 'a') {
						myMap.addTile(pos, ROAD, false);
					}
					else if (a[0] == 'f') {
						myMap.addTile(pos, FOREST, false);
					}
					else if (a[0] == 'h') {
						myMap.addTile(pos, HILL, false);
					}
					if (a[2] == 'i' && a[3] == 'n') {
						tmp = INFANTRY;
					}
					else if (a[2] == 'm' && a[3] == 'e') {
						tmp = MECH;
					}
					else if (a[2] == 'r' && a[3] == 'o') {
						tmp = ROCKET;
					}
					else if (a[2] == 'r' && a[3] == 'e') {
						tmp = RECON;
					}
					else if (a[2] == 'a' && a[3] == 'p') {
						tmp = APC;
					}
					else if (a[2] == 'a' && a[3] == 'a') {
						tmp = ANTIAIR;
					}
					else if (a[2] == 'a' && a[3] == 'r') {
						tmp = ARTILLERY;
					}
					else if (a[2] == 'm' && a[3] == 't') {
						tmp = MEDTANK;
					}
					else if (a[2] == 't' && a[3] == 'a') {
						tmp = TANK;
					}
					if (a[4] == '1') {
						if (iCreateMap) {
							tmp2 = myMap.getTeam();
						}
						else {
							tmp2 = myMap.getEnemyTeam();
						}
					}
					else if (a[4] == '2') {
						if (iCreateMap) {
							tmp2 = myMap.getEnemyTeam();
						}
						else {
							tmp2 = myMap.getTeam();
						}
					}
					myMap.addUnit(tmp, pos, tmp2);
				}
				else if (a.length() == 6 && a[2] == '+') {
					buildings_d temp;
					teams_d temp2;
					if (a[0] == 'c') {
						temp = CITY;
					}
					else if (a[0] == 'm') {
						temp = FACTORY;
					}
					if (a[1] == '0') {
						temp2 = NEUTRAL;
					}
					else if (a[1] == '1') {
						if (iCreateMap) {
							temp2 = myMap.getTeam();
						}
						else {
							temp2 = myMap.getEnemyTeam();
						}
					}
					else if (a[1] == '2') {
						if (iCreateMap) {
							temp2 = myMap.getEnemyTeam();
						}
						else {
							temp2 = myMap.getTeam();
						}
					}
					units_d tmp;
					teams_d tmp2;
					if (a[3] == 'i' && a[4] == 'n') {
						tmp = INFANTRY;
					}
					else if (a[3] == 'm' && a[4] == 'e') {
						tmp = MECH;
					}
					else if (a[3] == 'r' && a[4] == 'o') {
						tmp = ROCKET;
					}
					else if (a[3] == 'r' && a[4] == 'e') {
						tmp = RECON;
					}
					else if (a[3] == 'a' && a[4] == 'p') {
						tmp = APC;
					}
					else if (a[3] == 'a' && a[4] == 'a') {
						tmp = ANTIAIR;
					}
					else if (a[3] == 'a' && a[4] == 'r') {
						tmp = ARTILLERY;
					}
					else if (a[3] == 'm' && a[4] == 't') {
						tmp = MEDTANK;
					}
					else if (a[3] == 't' && a[4] == 'a') {
						tmp = TANK;
					}
					if (a[5] == '1') {
						if (iCreateMap) {
							tmp2 = myMap.getTeam();
						}
						else {
							tmp2 = myMap.getEnemyTeam();
						}
					}
					else if (a[5] == '2') {
						if (iCreateMap) {
							tmp2 = myMap.getEnemyTeam();
						}
						else {
							tmp2 = myMap.getTeam();
						}
					}
					myMap.addTile(pos, BUILDING, false);
					myMap.addBuilding(temp, temp2, pos);
					myMap.addUnit(tmp, pos, tmp2);
				}
			}
			i++;
			char * next_line();
		}
	}
}

Map Graphics::getMap() {
	return myMap;
}

units_d Graphics::chooseUnitToBuy(vector<units_d> available) {
	reDrawSide();
	bool inf = false, mech = false, reco = false, tank = false, medtank = false, apc = false, arti = false, rock = false, anti = false;
	for (int i = 0; i < (int)available.size(); i++) {
		switch (available[i]) {
		case INFANTRY:
			inf = true;
			break;
		case MECH:
			mech = true;
			break;
		case RECON:
			reco = true;
			break;
		case TANK:
			tank = true;
			break;
		case MEDTANK:
			medtank = true;
			break;
		case APC:
			apc = true;
			break;
		case ARTILLERY:
			arti = true;
			break;
		case ROCKET:
			rock = true;
			break;
		case ANTIAIR:
			anti = true;
			break;
		}
	}
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE, 0, "Buy a Unit!");
	int line = 2;
	if (inf) {
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * line, 0, "'I' to buy Infantry! (cost $1)");
		line++;
	}
	if (mech) {
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * line, 0, "'M' to buy Mech! (cost $3)");
		line++;
	}
	if (reco) {
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * line, 0, "'R' to buy Recon! (cost $4)");
		line++;
	}
	if (tank) {
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * line, 0, "'T' to buy Tank! (cost $7)");
		line++;
	}
	if (medtank) {
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * line, 0, "'D' to buy Medtank! (cost $16)");
		line++;
	}
	if (apc) {
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * line, 0, "'P' to buy APC! (cost $5)");
		line++;
	}
	if (arti) {
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * line, 0, "'Y' to buy Artillery! (cost $6)");
		line++;
	}
	if (rock) {
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * line, 0, "'K' to buy Rocket! (cost $15)");
		line++;
	}
	if (anti) {
		al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * line, 0, "'A' to buy Antiair! (cost $8)");
		line++;
	}
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * line, 0, "Press ESC to go back!");
	al_flip_display();

	ALLEGRO_EVENT ev;
	units_d answer = ERRORCLASS;
	while (answer ==  ERRORCLASS && graphicsError == G_NO_ERROR) {
		al_get_next_event(this->evQueue, &ev);
		switch (ev.type) {
		case ALLEGRO_EVENT_KEY_DOWN:
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_I:
				answer = INFANTRY;
				break;
			case ALLEGRO_KEY_M:
				answer = MECH;
				break;
			case ALLEGRO_KEY_R:
				answer = RECON;
				break;
			case ALLEGRO_KEY_T:
				answer = TANK;
				break;
			case ALLEGRO_KEY_D:
				answer = MEDTANK;
				break;
			case ALLEGRO_KEY_P:
				answer = APC;
				break;
			case ALLEGRO_KEY_Y:
				answer = ARTILLERY;
				break;
			case ALLEGRO_KEY_K:
				answer = ROCKET;
				break;
			case ALLEGRO_KEY_A:
				answer = ANTIAIR;
				break;
			case ALLEGRO_KEY_ESCAPE:
				answer = NO_CLASS;
				break;
			default:
				answer = ERRORCLASS;
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			graphicsError = G_GAME_CLOSED;
			break;
		default:
			break;
		}
	}
	reDrawSide();
	return answer;
}

bool Graphics::checkIfUserClose() {
	ALLEGRO_EVENT ev;
	bool tmp = false;
	al_get_next_event(evQueue, &ev);
	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		tmp = true;
		graphicsError = G_GAME_CLOSED;
	}
	else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
		switch (ev.keyboard.keycode) {
		case ALLEGRO_KEY_ESCAPE:
			tmp = true;
			break;
		}
	}
	return tmp;
}

void Graphics::drawSpecificMessage(string message,unsigned int time) {
	if (graphicsError == G_NO_ERROR) {
		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 6, TILE_SIDE * 5, 0, message.c_str());
		al_flip_display();
		timerMiliseconds(time);
		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_flip_display();
	}
	return;
}

int Graphics::getChecksum() {
	return checksum;
}