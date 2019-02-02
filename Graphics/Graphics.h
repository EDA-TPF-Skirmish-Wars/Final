//////////////////////////////////////////////////////////
//Libreria Graphics                                     //
//Encargada de manejar toda la parte grafica del juego  //
//skirmich Wars. TP FINAL EDA, 1er Cuatrimestre 2017    //
//Trabajo Hecho por:                                    //
//  -Ian Diaz                                           //
//  -Martina Maspero                                    //
//                                          26/08/2018  //
//////////////////////////////////////////////////////////
/*Definiciones:
-   Las posiciones son discretas y estan horizontalmente de 0 a 15 y verticalmente de 0-11.
    La posicion (0,0) se encuentra en la esquina superior izquierda del mapa y la posicion
    (15,11) en la esquina inferior derecha.

-   Las clases "terrenos", "edificios" y "unidades" deben tener, aparte de las que se necesite,
    los metodos que poseen las clases de 'dummy.h'
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H


#include <iostream>
#include <vector>
#include <string>
//#define DEBUG
//#define PLAYMUSIC
//#define SHOW_INTRODUCTION
//#define FOW

#ifdef  DEBUG
    #include "./dummys/dummys.h"
#endif

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "../Game/classMap.h"
#include "../Game/classBuilding.h"
#include "../Game/TileClass.h"
#include "../Game/classUnit.h"
#include "../Game/Position.h"



typedef enum {A_NO_ACTION, A_ATTACK, A_MOVE , A_PURCHASE, A_PASS ,A_CLOSE_GAME}act_s;

typedef enum {G_LOAD_GRAPHICS_ERROR, G_NO_ERROR,G_LOAD_BITMAP_ERROR, G_DISPLAY_ERROR, G_LOAD_FONT_ERROR, G_GAME_CLOSED, G_AUDIO_ERROR}errors_s;

//typedef struct{
//    position_s positionFrom;
//    position_s positionTo;
//    Unit unitType;
//}movement_s;

typedef struct {
	act_s act;
	Position positionFrom;
	Position positionTo;
}action_s;



class Graphics
{
public:
	Graphics();//Funciona perfecta
	/*Funcion de inicializacion de la clase
	NO devuelve nada.
	*/
    ~Graphics();//Funciona perfecta

    errors_s updateGraphics(Map newMap); //Funciona perfecta
	/*  Esta funcion se la utiliza para actualizar los vectores de unidades y de edificios que fueron modificados por el
	jugador oponente. Se le pasa el nuevo mapa, con los nuevos vectores de unidades y edificios que actualiza el vector contenido
	dentro de esta clase. Y una vez actualizado grafica todas las lineas del mapa para que se vean en pantalla.
	Devuelve: un errors_s que estan especificados al principio de este archivo
	*/
    action_s getUserAction();//Funciona perfecta
	/*Funcion encargada de obtener los movimientos que haga el usuario.
	Devuelve: una action_s
	*/

	errors_s getError();
	/*Funcion encargada de devolver un error_s, en caso de devolver algo distinto a G_NO_ERROR, se debe proceder a cerrar el juego
	ya que hubo un error que no pudo ser resuelto dentro de la parte grafica. Si se quiere saber que tipo de error ocurrio, se tiene
	que mirar el valor de la respuesta de la funcion ya que especifica ue tipo de error ocurrio.
	*/

	void displayActionInvalid();//Funciona perfecta
	/*Funcion encargada de mostrar un POP-UP que dice "Accion Invalida", sirve para cuando el usuario toca una accion la cual no esta
	permitida por las reglas del juego. Es bloqueante.
	*/

	void showDices(int yours, int enemys);//Funciona perfecta
	/*Funcion encargada de mostrar el resultado de los dados obtenido en alguna parte del juego, es bloqueante.
	Recibe el valor de los dados tirados, y una variable que debe ser True si el usuario gano y false si perdio, esto
	esta para definir en los casos de empate.
	*/

	string getName();

	void setEnemyName(string enemyName);

	string chooseMap();//Funciona perfecta

	void selectMap(string mapName, int checksum, bool iCreateMap);

	Map getMap();
	
	units_d chooseUnitToBuy();

	bool checkIfUserClose();

protected:
    Map myMap;
	string myName;
	string enemyName;
private:
    void showLine(unsigned int i); //Funciona perfecta
	/*Funcion encargada de dibujar en el display la linea numero i contando de arriba hacia abajo, es decir la linea
	numero 0 es la linea horizontal superior y la linea numero 11  es la linea horizontal inferior.*/
    void drawTerrain(terrains_d terrain2Draw, Position pos);//Funciona perfecta
    void drawBuilding(Building buildingToDraw);//Funciona perfecta
    void drawUnit(Unit unitToDraw);//Funciona perfecta
    void showTransition();//Funciona perfecta
    action_s getMouseAction(void);//Funciona perfecta
    action_s getKeyboardAction(int xTile, int yTile);//Funciona perfecta
    action_s showPopUp(options_s opt, int xTile, int yTile); //Funciona perfecta
	void drawMap();//Funciona perfecta, un poco lenta
	void drawMessage();//Funciona perfecta
	string getBuildingImagePath(int typeBuild, int team);//Funciona perfecta
	string getTerrainImagePath(terrains_d terrain, Position pos);//Funciona perfecta HAY QUE HACERLA DE VUELTA DE CERO
	string getUnitImagePath(int typeUnit, int team);//Funciona perfecta
	void reDrawSide();//Funciona perfecta
	void introduction(); //Funciona perfecta
	string dispChoose();//Funciona perfecta
	void setTeam();//Funciona perfecta

    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * evQueue = NULL;
    ALLEGRO_FONT *font = NULL;
	ALLEGRO_FONT *fontLarge = NULL;
	errors_s graphicsError;
	ALLEGRO_SAMPLE_INSTANCE *sample = NULL;
	ALLEGRO_SAMPLE * sample_data = NULL;
	ALLEGRO_VOICE *voice;
	ALLEGRO_MIXER *mixer;
};

#endif