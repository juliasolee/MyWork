#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "stage.h"

#include "../includes.h"
#include "../camera.h"

class Billboard;
class Texture;
class Mesh;

class Button
{
public:
	Vector2 center;
	float width;
	float height;
	Vector4 uvs;
	Texture* texture;
	float image_size;
	int id;
	bool used;

	Button(Vector2 center_, Vector4 uvs_, int id_, Texture* texture_, float image_size_);
	void resize(float width_, float height_);
	void setCenter(Vector2 center_);
	bool doButton();
};

class Minigame
{
public:
	Minigame();

	bool solution1[16];
	bool player1[16];
	int solution2[3];
	int player2[3];
	std::vector <Vector2> positions_quad; //Vector de posicions on es coloquen les imatges del minigame 1
	std::vector <Vector2> positions_mouse; //Vector de posicions per mirar el mouse_pos
	Vector2 positions_quad2[3]; //Vector de posicions on es coloquen les imatges del minigame 2


	int width;
	int height;

	void resize();
	int playMinigame1();
	int playMinigame2();
	void paintQuad(const char* texture, Mesh mesh, Camera cam2D);

};

class Gameplay : public Stage
{
public:

	Gameplay();
	virtual void render();
	virtual void update(double seconds_elapsed);

	float time; //Temps de joc

	std::vector<Button*> buttons;//Conjunt de botos que s'inicialitza al principi
	std::vector<Button*> buttons_GUI;//S'aniran guardant els buttons que s'han d'anar col.locant a la GUI

	Minigame* minigame;
	bool play_minigame1;
	int win_minigame1;
	bool play_minigame2;
	int win_minigame2;

	bool paint_GUI;

	bool last_seconds;

	Camera* camera; //our global camera
	Camera* top_camera; //our top view camera

	void init_game();
	void game_play();

	int shells;//numero de petxines que hi ha en tot moment

	int last_near_billboard;//index de l'ultim pla proper
	int last_id_touched;//id de l'ultim boto seleccionat

	bool load();

	void render_environment(Camera* camera);
	void create_sea(Camera* camera);

	void resizeButtons(int width);
	void draw_test_GUI(std::vector<Billboard*> billboards, int index_billboard);
	void initButtons();

	//Gameplay functions
	void grab_bottle(int near_index);
	void break_coco(int id_button);
	void delete_knife();
	void place_shell();
	void open_chest(int near_index);
	void open_tent(int near_index);
	void fish(int near_index);
	void end();

	//Testing classe
	void show();
};

#endif 