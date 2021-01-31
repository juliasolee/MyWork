#include "stage.h"
#include "gameplay.h"
#include "../shader.h"
#include "../mesh.h"
#include "../texture.h"
#include "../game.h"
#include "../audiomanager.h"
#include "../input.h"

Stage::Stage() {}
void Stage::render() {}
void Stage::update(double seconds_elapsed) {}

Intro::Intro() 
{
	n_intro = 0;

	int w = Game::instance->window_width;
	int h = Game::instance->window_height;

	next = new Button(Vector2(w - (w/8), (w/8)), Vector4(0, 0, 1024, 1024), 0, Texture::Get("data/stages/intro/next.tga"), 1024);
	next->resize((w/4), (w / 4));
}
void Intro::render()
{

	//Recoloquem el boto per si es canvia la mida de la pantalla
	int w = Game::instance->window_width;
	int h = Game::instance->window_height;

	next->setCenter(Vector2(w - (w / 8), (w / 8)));

	const char* texture;
	if (n_intro == 0) {
		texture = "data/stages/intro/intro_1.tga";
	}
	else if (n_intro == 1)
	{
		texture = "data/stages/intro/intro_2.tga";
	}
	else if (n_intro == 2) {
		texture = "data/stages/intro/intro_3.tga";
	}
	else if (n_intro == 3)
	{
		texture = "data/stages/intro/intro_4.tga";
	}
	else if (n_intro == 4) {
		texture = "data/stages/intro/intro_5.tga";
	}
	else if (n_intro == 5)
	{
		texture = "data/stages/intro/intro.tga";
	}
	
	if (n_intro == 5 && Input::isKeyPressed(SDL_SCANCODE_SPACE))
	{
		Game::instance->current_stage = Game::instance->menu;
	}

	//We paint the scene
	Mesh mesh;
	mesh.createQuad(Game::instance->window_width / 2, Game::instance->window_height / 2, Game::instance->window_width, Game::instance->window_height, false);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, 0, Game::instance->window_height, 0, 10000);

	Matrix44 model;

	Shader* shader = Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs");
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", Texture::Get(texture));
	mesh.render(GL_TRIANGLES);
	shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	if (n_intro != 5) {
		//Paint the button
		if (next->doButton() == true) //Controls
		{
			Game::instance->mouse_pos = Vector2(0, 0);
			n_intro = n_intro + 1;
		}
	}

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);
}
void Intro::update(double seconds_elapsed) {}

Menu::Menu()
{
	int w = Game::instance->window_width;
	int h = Game::instance->window_height;
	//Init the 3 buttons: controls, difficulty and play
	//Controls [0]
	Texture* aux_t = Texture::Get("data/stages/menu/controls.tga");
	Button* aux_b = new Button(Vector2(w / 2, (h / 6 + h / 3 * 2)), Vector4(0, 0, 1024, 1024), 0, aux_t, 1024);
	aux_b->resize(300, 300);
	buttons.push_back(aux_b);
	//Difficulty [1]
	aux_t = Texture::Get("data/stages/menu/difficulty.tga");
	aux_b = new Button(Vector2(w / 2, (h / 6 + h / 3)), Vector4(0, 0, 1024, 1024), 1, aux_t, 1024);
	aux_b->resize(300, 300);
	buttons.push_back(aux_b);
	//Play [2]
	aux_t = Texture::Get("data/stages/menu/play.tga");
	aux_b = new Button(Vector2(w / 2, h / 6), Vector4(0, 0, 1024, 1024), 2, aux_t, 1024);
	aux_b->resize(300, 300);
	buttons.push_back(aux_b);

	//Easy [3]
	aux_t = Texture::Get("data/stages/menu/easy.tga");
	aux_b = new Button(Vector2(w / 2, (h / 6 + h / 3 * 2)), Vector4(0, 0, 1024, 1024), 3, aux_t, 1024);
	aux_b->resize(300, 300);
	buttons.push_back(aux_b);
	//Medium [4]
	aux_t = Texture::Get("data/stages/menu/medium.tga");
	aux_b = new Button(Vector2(w / 2, (h / 6 + h / 3)), Vector4(0, 0, 1024, 1024), 4, aux_t, 1024);
	aux_b->resize(300, 300);
	buttons.push_back(aux_b);
	//Difficult [5]
	aux_t = Texture::Get("data/stages/menu/hard.tga");
	aux_b = new Button(Vector2(w / 2, h / 6), Vector4(0, 0, 1024, 1024), 5, aux_t, 1024);
	aux_b->resize(300, 300);
	buttons.push_back(aux_b);

	//Pantalla de controls [6]
	aux_t = Texture::Get("data/stages/menu/cont.tga");
	aux_b = new Button(Vector2(w / 2, h / 2), Vector4(0, 0, 1024, 1024), 6, aux_t, 1024);
	aux_b->resize(w, h);
	buttons.push_back(aux_b);

	last_index = -1;

	//Inicialitzem el temps de joc a nivell medium per si el jugador no ho tria
	Game::instance->mouse_pos = Vector2(0, 0);
	Game::instance->max_time = 1200;
	Game::instance->range_time = 180;
}
void Menu::moveButtons()
{
	int w = Game::instance->window_width;
	int h = Game::instance->window_height;

	buttons[0]->setCenter(Vector2(w / 2, (h / 6 + h / 3 * 2)));
	buttons[1]->setCenter(Vector2(w / 2, (h / 6 + h / 3)));
	buttons[2]->setCenter(Vector2(w / 2, h / 6));
	buttons[3]->setCenter(Vector2(w / 2, (h / 6 + h / 3 * 2)));
	buttons[4]->setCenter(Vector2(w / 2, (h / 6 + h / 3)));
	buttons[5]->setCenter(Vector2(w / 2, h / 6));
	buttons[6]->setCenter(Vector2(w / 2, h / 2));
	
}
void Menu::render()
{
	moveButtons();
	//Pintem el fons
	draw_bg();

	if (buttons[0]->used == false) {
		if (buttons[0]->doButton() == true) //Controls
		{
			Audiomanager::instance->PlaySound("data/sounds/select.wav", false);//Soroll seleccionant
			Game::instance->mouse_pos = Vector2(0, 0);
			last_index = 0;
			buttons[0]->used = true;
			buttons[1]->used = true;
			buttons[2]->used = true;
		}
	}
	if (buttons[1]->used == false) {
		if (buttons[1]->doButton() == true) //Difficulty
		{
			Audiomanager::instance->PlaySound("data/sounds/select.wav", false);//Soroll seleccionant
			Game::instance->mouse_pos = Vector2(0, 0);
			last_index = 1;
			buttons[0]->used = true;
			buttons[1]->used = true;
			buttons[2]->used = true;
		}
	}
	if (buttons[2]->used == false) {
		if (buttons[2]->doButton() == true) //Play
		{
			Audiomanager::instance->PlaySound("data/sounds/select.wav", false);//Soroll seleccionant
			
																			   //El temps passa a 0
			Game::instance->dif_time = Game::instance->time;

			//Es comença el joc
			Game::instance->current_stage = Game::instance->loading;
		}
	}

	if (last_index == 0) //Difficulty
	{
		draw_controls();
	}
	else if (last_index == 1) //Difficulty
	{
		//Es mostren les dificultats disponibles i depenent de les que es trii, els temps son uns o uns altres
		choose_difficulty();
	}

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);
}

void Menu::update(double seconds_elapsed) {}

void Menu::draw_bg()
{
	Mesh mesh;
	mesh.createQuad(Game::instance->window_width / 2, Game::instance->window_height / 2, Game::instance->window_width, Game::instance->window_height, false);
	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, 0, Game::instance->window_height, 0, 10000);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Paint the background
	Matrix44 model;
	Shader* shader = Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs");
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", Texture::Get("data/stages/menu/menu.tga"));
	mesh.render(GL_TRIANGLES);
	shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
void Menu::draw_controls()
{
	//Pintem la pantalla de controls
	bool nothing = buttons[6]->doButton();

	if (Game::instance->mouse_pos.x != 0 && Game::instance->mouse_pos.y != 0) //Si es clica a algun lloc de la pantalla de deixen de veure els controls
	{
		Game::instance->mouse_pos = Vector2(0, 0);
		buttons[0]->used = false;
		buttons[1]->used = false;
		buttons[2]->used = false;
		last_index = -1;
	}

}
void Menu::choose_difficulty()
{

	//Depenent de quina de les 3 dificultats es trii: facil, normal, dificil. S'estableixen uns temps
	if (buttons[3]->doButton() == true) //Easy
	{
		Audiomanager::instance->PlaySound("data/sounds/select.wav", false);//Soroll seleccionant

		Game::instance->mouse_pos = Vector2(0, 0);
		Game::instance->max_time = 1500;
		Game::instance->range_time = 300;
		buttons[0]->used = false;
		buttons[1]->used = false;
		buttons[2]->used = false;
		last_index = -1;
	}
	if (buttons[4]->doButton() == true) //Medium
	{
		Audiomanager::instance->PlaySound("data/sounds/select.wav", false);//Soroll seleccionant

		Game::instance->mouse_pos = Vector2(0, 0);
		Game::instance->max_time = 1200;
		Game::instance->range_time = 180;
		buttons[0]->used = false;
		buttons[1]->used = false;
		buttons[2]->used = false;
		last_index = -1;
	}
	if (buttons[5]->doButton() == true) //Hard
	{
		Audiomanager::instance->PlaySound("data/sounds/select.wav", false);//Soroll seleccionant

		Game::instance->mouse_pos = Vector2(0, 0);
		Game::instance->max_time = 900;
		Game::instance->range_time = 90;
		buttons[0]->used = false;
		buttons[1]->used = false;
		buttons[2]->used = false;
		last_index = -1;
	}
}

Loading::Loading()
{


}
void Loading::render()
{
	draw_bg();

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);

	if (Game::instance->gameplay->load() == true) {

		Game::instance->current_stage = Game::instance->gameplay;
	}

}
void Loading::update(double seconds_elapsed) {}
void Loading::draw_bg()
{
	Mesh mesh;
	mesh.createQuad(Game::instance->window_width / 2, Game::instance->window_height / 2, Game::instance->window_width, Game::instance->window_height, false);
	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, 0, Game::instance->window_height, 0, 10000);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Paint the background
	Matrix44 model;
	Shader* shader = Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs");
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", Texture::Get("data/stages/loading/loading.tga"));
	mesh.render(GL_TRIANGLES);
	shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

End::End()
{
	int w = Game::instance->window_width;
	menu = new Button(Vector2((w/8), (w/8)), Vector4(0, 0, 1024, 1024), 0, Texture::Get("data/stages/end/menu.tga"), 1024);
	menu->resize((w / 4), (w / 4));

}
void End::render()
{
	
	int w = Game::instance->window_width;
	menu->setCenter(Vector2(w /8,w / 8));
	//Mirem quin tipus d'ending tindrem:
	float time = Game::instance->gameplay->time;
	float max_time = Game::instance->max_time;
	float range_time = Game::instance->range_time;

	const char* texture;
	
	if (time >= max_time) //1: ha perdut -- Si t > max_time -- mor de gana a la illa
	{
		Audiomanager::instance->PlaySound("data/sounds/wrong.wav", false);//Ha guanyat
		texture = "data/stages/end/end_1.tga";
	}
	else if (time < max_time && time >= max_time - range_time) // Si t < max_time pero t > max_time - rang_time -- mor de gana al mar
	{
		Audiomanager::instance->PlaySound("data/sounds/wrong.wav", false);//Ha guanyat
		texture = "data/stages/end/end_2.tga";
	}
	else if (time < max_time - range_time && time >= max_time - range_time*2) // Si t < max_time - rang_time pero t > max_time - rang_time*2 -- sel mengen taurons
	{
		Audiomanager::instance->PlaySound("data/sounds/wrong.wav", false);//Ha guanyat
		texture = "data/stages/end/end_3.tga";
	}
	else if (time < max_time - range_time*2) // Si t <  max_time - rang_time*2 -- arriba a casa
	{
		Audiomanager::instance->PlaySound("data/sounds/win_game.wav", false);//Ha guanyat
		texture = "data/stages/end/end_4.tga";
	}

	Mesh mesh;
	mesh.createQuad(Game::instance->window_width / 2, Game::instance->window_height / 2, Game::instance->window_width, Game::instance->window_height, false);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, 0, Game::instance->window_height, 0, 10000);

	Matrix44 model;

	Shader* shader = Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs");
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", Texture::Get(texture));
	mesh.render(GL_TRIANGLES);
	shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	if (menu->doButton() == true) {
		Game::instance->current_stage = Game::instance->menu;
	}

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);
}
void End::update(double seconds_elapsed) {}
