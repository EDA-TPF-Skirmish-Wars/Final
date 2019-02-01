#include "./Graphics.h"
#include "../Network/Timer.h"
#include "./CSVParser/CSVParser.h"
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
	}*/
	if (graphicsError == G_NO_ERROR) {
		drawMap();
	}
	if (graphicsError == G_NO_ERROR) {
		al_flip_display();
	}

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
	showTransition();
	drawMap();
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
	if (graphicsError == G_NO_ERROR) {
#ifdef FOW
		if (terrainToDraw.getFog() == false) {
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
		if (buildingToDraw.getFog() == false || buildingToDraw.getTeam() == myMap.getTeam()) {
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
		if (unitToDraw.getFog() == false || unitToDraw.getTeam() == myMap.getTeam()) {
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

action_s Graphics::getUserAction(){
    action_s action;
	action.act = A_NO_ACTION;
	drawMessage();
	action = getMouseAction();
	if (action.act == A_CLOSE_GAME) {
		al_destroy_display(display);
		display = NULL;
		graphicsError = G_GAME_CLOSED;
	}
    return action;
}

action_s Graphics::getMouseAction(){
    ALLEGRO_EVENT ev;
    bool tmp = true;
    action_s temp;
	if (graphicsError == G_NO_ERROR) {
		while (tmp) {
			al_get_next_event(this->evQueue, &ev);
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
			temp = showPopUp(myMap.getOptions(pos), xTile, yTile);
		}
		else
			temp.act = A_CLOSE_GAME;
	}
	else {
		temp.act = A_CLOSE_GAME;
	}
    return temp;
}

action_s Graphics::showPopUp(options_s opt, int xTile, int yTile){
	reDrawSide();
    int amountOfLines = 1;
	if (graphicsError == G_NO_ERROR) {
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
		al_flip_display();
	}

    return getKeyboardAction(xTile,yTile);

}

action_s Graphics::getKeyboardAction(int xTile, int yTile){
    ALLEGRO_EVENT ev;
    action_s action;
    action.act = A_NO_ACTION;
    while(action.act == A_NO_ACTION){
        al_get_next_event(this->evQueue,&ev);
        switch(ev.type){
            case ALLEGRO_EVENT_KEY_DOWN:
                switch(ev.keyboard.keycode){
                    case ALLEGRO_KEY_W:
                        action.act = A_ATTACK;
                        action.positionFrom.column = xTile;
                        action.positionFrom.row = yTile; 
                        action.positionTo.column = xTile;
                        action.positionTo.row = yTile-1;
                        break;
                    case ALLEGRO_KEY_A:
                        action.act = A_ATTACK;
                        action.positionFrom.column = xTile;
                        action.positionFrom.row = yTile; 
                        action.positionTo.column = xTile-1;
                        action.positionTo.row = yTile;
                        break;
                    case ALLEGRO_KEY_S:
                        action.act = A_ATTACK;
                        action.positionFrom.column = xTile;
                        action.positionFrom.row = yTile; 
                        action.positionTo.column = xTile;
                        action.positionTo.row = yTile+1;
                        break;
                    case ALLEGRO_KEY_D:
                        action.act = A_ATTACK;
                        action.positionFrom.column = xTile;
                        action.positionFrom.row = yTile; 
                        action.positionTo.column = xTile+1;
                        action.positionTo.row = yTile;
                        break;
                    case ALLEGRO_KEY_B:
                        action.act = A_PURCHASE;
                        break;
                    case ALLEGRO_KEY_P:
                        action.act = A_PASS;
                        break;
                    case ALLEGRO_KEY_UP:
                        action.act = A_MOVE;
                        action.positionFrom.column = xTile;
						action.positionFrom.row = yTile;
						action.positionTo.column = xTile;
						action.positionTo.row = yTile-1;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        action.act = A_MOVE;
						action.positionFrom.column = xTile;
						action.positionFrom.row = yTile;
						action.positionTo.column = xTile;
						action.positionTo.row = yTile+1;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        action.act = A_MOVE;
						action.positionFrom.column = xTile;
						action.positionFrom.row = yTile;
						action.positionTo.column = xTile-1;
						action.positionTo.row = yTile;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        action.act = A_MOVE;
						action.positionFrom.column = xTile;
						action.positionFrom.row = yTile;
						action.positionTo.column = xTile+1;
						action.positionTo.row = yTile;
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
			timerMiliseconds(10);
		}
		timerMiliseconds(10);
		for (unsigned int i = 0; i < 255; i++) {
			al_clear_to_color(al_map_rgb(i, i, i));
			al_draw_text(fontLarge, al_map_rgb((255-i),(255-i), (255-i)), TILE_SIDE * 3, TILE_SIDE*5, 0, 
				"Some changes have been happening during night!");
			al_flip_display();
			timerMiliseconds(10);
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
		al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 5, TILE_SIDE, 0, "The Dices Were:");
		string str1, str2, str3, str;
		str1 = std::to_string(yours);
		str2 = std::to_string(enemys);
		str = str1 + "   " + str2;
		al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 7, TILE_SIDE * 2, 0, str.c_str());
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
		answer = "./resources/Images/units/antiair_";
		break;
	case APC:
		answer = "./resources/Images/units/apc_";
		break;
	case ARTILLERY:
		answer = "./resources/Images/units/artillery_";
		break;
	case INFANTRY:
		answer = "./resources/Images/units/infantry_";
		break;
	case MECH:
		answer = "./resources/Images/units/mech_";
		break;
	case MEDTANK:
		answer = "./resources/Images/units/medtank_";
		break;
	case RECON:
		answer = "./resources/Images/units/recon_";
		break;
	case ROCKET:
		answer = "./resources/Images/units/rocket_";
		break;
	case TANK:
		answer = "./resources/Images/units/tank_";
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
		answer = "./resources/Images/building/city_";
		break;
	case FACTORY:
		answer = "./resources/Images/building/factory_";
		break;
	case HQ:
		answer = "./resources/Images/building/hq_";
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
		answer = "./resources/Images/terrain/forest.png";
	}
	else if (terrain == HILL) {
		answer = "./resources/Images/terrain/hill.png";
	}
	else if (terrain == GRASS) {
		answer = "./resources/Images/terrain/plain.png";
	}
	else if (terrain == RIVER) { //RIO
		answer = "./resources/Images/terrain/";
		bool isThereOneUp, isThereOneDown, isThereOneLeft, isThereOneRight;
		isThereOneUp = false;
		isThereOneDown = false;
		isThereOneLeft = false;
		isThereOneRight = false;
		Position pos1(pos.row - 1, pos.column);
		if (myMap.getTerrain(pos1) == RIVER) {
			isThereOneUp = true;
		}
		Position pos2(pos.row + 1, pos.column);
		if (myMap.getTerrain(pos2) == RIVER) {
			isThereOneDown = true;
		}
		Position pos3(pos.row, pos.column - 1);
		if (myMap.getTerrain(pos3) == RIVER) {
			isThereOneLeft = true;
		}
		Position pos4(pos.row, pos.column + 1);
		if (myMap.getTerrain(pos4) == RIVER) {
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
		answer = "./resources/Images/terrain/";
		bool isThereOneUp, isThereOneDown, isThereOneLeft, isThereOneRight;
		isThereOneUp = false;
		isThereOneDown = false;
		isThereOneLeft = false;
		isThereOneRight = false;
		Position pos1(pos.row - 1, pos.column);
		if (myMap.getTerrain(pos1) == ROAD) {
			isThereOneUp = true;
		}
		Position pos2(pos.row + 1, pos.column);
		if (myMap.getTerrain(pos2) == ROAD) {
			isThereOneDown = true;
		}
		Position pos3(pos.row, pos.column - 1);
		if (myMap.getTerrain(pos3) == ROAD) {
			isThereOneLeft = true;
		}
		Position pos4(pos.row, pos.column + 1);
		if (myMap.getTerrain(pos4) == ROAD) {
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
	string str = "./resources/pop-up.png";
	ALLEGRO_BITMAP *bmp = al_load_bitmap(str.c_str());
	al_draw_scaled_bitmap(bmp, 0, 0, 575, 600, TILE_SIDE * 16, -24, TILE_SIDE * 6.6, TILE_SIDE *13.5, 0);
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
	selectMap(str, 0);
	return str;
}

string Graphics::dispChoose() {
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 4,TILE_SIDE * 5, 0, "Press a number from 1 to 10 to choose a Map!");
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
				str = "./resources/maps/BalancedArena.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_2:
				str = "./resources/maps/BalancedRing.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_3:
				str = "./resources/maps/BalancedCross.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_4:
				str = "./resources/maps/IslandWar.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_5:
				str = "./resources/maps/MystPi.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_6:
				str = "./resources/maps/Nascar.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_7:
				str = "./resources/maps/SanFranciscoBridge.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_8:
				str = "./resources/maps/SnakeArena.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_9:
				str = "./resources/maps/SuperS.csv";
				tmp = false;
				break;
			case ALLEGRO_KEY_0:
				str = "./resources/maps/WaterWorld.csv";
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
	al_draw_text(fontLarge, al_map_rgb(0, 0, 0), TILE_SIDE * 6, TILE_SIDE * 5, 0, "Choose a color for your team!");
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
}

string Graphics::getName() {
	string name;
	name = "noname";
	return name;
}

void Graphics::selectMap(string mapName, int checksum) {
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
					myMap.addBuilding(FACTORY, NEUTRAL, pos);
				}
				else if (a == "m1") {
					myMap.addBuilding(FACTORY, myMap.getTeam(), pos);
				}
				else if (a == "m2") {
					myMap.addBuilding(FACTORY, myMap.getEnemyTeam(), pos);
				}
				else if (a == "c0") {
					myMap.addBuilding(CITY, NEUTRAL, pos);
				}
				else if (a == "c1") {
					myMap.addBuilding(CITY, myMap.getTeam(), pos);
				}
				else if (a == "c2") {
					myMap.addBuilding(CITY, myMap.getEnemyTeam(), pos);
				}
				else if (a == "q1") {
					myMap.addBuilding(HQ, myMap.getTeam(), pos);
				}
				else if (a == "q2") {
					myMap.addBuilding(HQ, myMap.getEnemyTeam(), pos);
				}
			}
			else if (a.length() == 5 && a[1] == '+') {
				units_d tmp;
				teams_d tmp2;
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
					tmp2 = myMap.getTeam();
				}
				else if (a[4] == '2') {
					tmp2 = myMap.getEnemyTeam();
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
					temp2 = myMap.getTeam();
				}
				else if (a[1] == '2') {
					temp2 = myMap.getEnemyTeam();
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
					tmp2 = myMap.getTeam();
				}
				else if (a[5] == '2') {
					tmp2 = myMap.getEnemyTeam();
				}


				myMap.addBuilding(temp, temp2, pos);
				myMap.addUnit(tmp, pos, tmp2);
			}
		}
		i++;
		char * next_line();
	}
}

Map Graphics::getMap() {
	return myMap;
}

units_d Graphics::chooseUnitToBuy() {
	reDrawSide();
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE, 0, "Buy a Unit!");
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 2, 0, "'I' to buy Infantry!");
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 3, 0, "'M' to buy Mech!");
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 4, 0, "'R' to buy Recon!");
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 5, 0, "'T' to buy Tank!");
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 6, 0, "'D' to buy Medtank!");
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 7, 0, "'P' to buy APC!");
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 8, 0, "'Y' to buy Artillery!");
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 9, 0, "'K' to buy Rocket!");
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 10, 0, "'A' to buy Antiair!");
	al_draw_text(font, al_map_rgb(0, 0, 0), TILE_SIDE * 17, TILE_SIDE * 11, 0, "Press ESC to go back!");
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