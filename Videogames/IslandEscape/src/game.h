/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "stages/gameplay.h"
#include "stages/stage.h"

class Game
{
public:
	static Game* instance;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;
	Vector2 mouse_pos;

	//some globals
	long frame;
    float time;
	float elapsed_time;
	int fps;
	bool must_exit;

	Camera* camera; //our global camera

	bool mouse_locked; //tells if the mouse is locked (not seen)

	Game( int window_width, int window_height, SDL_Window* window );

	//main functions
	void render( void );
	void update( double dt );

	//events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);

	//STAGES
	Stage* current_stage;
	Intro* intro;
	Menu* menu;
	Loading* loading;
	Gameplay* gameplay;
	End* end;

	//TIMES
	//Aixo sera el temps maxim del joc i el rang entre els temps que defineixen els resultats (en s)
	//Dependra de la dificultad
	float dif_time; //Diferencia entre el time i el moment en el que s'ha començat el joc
	float max_time;
	float range_time;

};

#endif 