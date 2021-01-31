#include "stage.h"
#include "../utils.h"
#include "../mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "scene.h"
#include "audiomanager.h"

#include <cmath>

Stage::Stage() {}
void Stage::render() {}
void Stage::update(double seconds_elapsed) {}

/************/
/**GAMEPLAY**/
/************/
Gameplay::Gameplay() {

	//We create the world
	new World();

	//We create the audio manager
	new Audiomanager();

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	//create our top view camera
	top_camera = new Camera();
	top_camera->lookAt(Vector3(500.f, 100.f, 100.f), Vector3(500.f, 0.f, 500.f), Vector3(1.f, 0.f, 0.f)); //position the camera and point to 500,0,500
	top_camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	minigame = new Minigame();
	play_minigame1 = false;
	win_minigame1 = -1;
	play_minigame2 = false;
	win_minigame2 = -1;

	//create a plane mesh
	floormesh = new Mesh();
	floormesh->createSubdividedPlane(1000, 10, false);

	//create a plane mesh for the transparent water
	water_plane = new Mesh();
	water_plane->createPlane(10000);

	//we load the mask image
	mask = new Image();
	//mask->fromTexture(Texture::Get("data/island/test_mask.tga"));
	mask->loadTGA("data/island/island_mask.tga");

	//we load the test mask image
	test_mask = new Image();
	//test_mask->fromTexture(Texture::Get("data/island/prova_mask.tga"));
	test_mask->loadTGA("data/island/test_mask.tga");
	//Creating the sky
	sky = new Entity(Mesh::Get("data/sky.ASE"), Texture::Get("data/cielo.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), m, 0);

	initButtons();

	World::instance->entities.push_back(sky);
	World::instance->mask = mask;
	World::instance->test_mask = test_mask;
	World::instance->createWorld(camera);

	//Audio del game
	Audiomanager::instance->PlaySound("data/sounds/island_ambient.wav", true);

}

void Gameplay::render() {

}

void Gameplay::update(double seconds_elapsed)
{

}
void Gameplay::game_play() {

	//std::cout << "entra\n" << std::endl;
	World* world = World::instance;

	int near_index = world->player->isNear(world->billboards);//Retorna la posicio del pla mes proper dins el vector de plans


	//TODO: vector billboards per referencia:
	std::vector<Billboard*> &billboards = World::instance->billboards;



	if (near_index != -1) {//Si existeix un pla que esta a la distancia minima del character

		Billboard* near_billboard = world->billboards.at(near_index);//Guardem el pla

		std::cout << "\nAprop de id:" << near_billboard->index << "\n" << std::endl;

		if (near_billboard->used == true) {//Si ja s'ha utilitzat no farem res
			return;
		}

		if (near_billboard->index == 0) {//si el pla proper es una palm tree

			if (buttons[13]->doButton() == true) {//Si el boto SHAKE es clica

				int count_cocos = 0;

				for (int i = 0; i < buttons_GUI.size(); i++) {//Recorrem el vector de buttons_GUI per veure si hi ha algun coco (hi poden haver 2)

					if (buttons_GUI[i]->id == 0) {
						//si ja hem col.locat el coco 0, podrem col.locar el coco 1
						count_cocos++;
					}
				}

				if (count_cocos == 0) {//Si no hi ha cap coco col.loquem el coco 0

					Audiomanager::instance->PlaySound("data/sounds/shaking_palmera.wav", false);//Audio shaking palmera

					buttons_GUI.push_back(buttons[0]);
					std::cout << "\nCoco num 0 afegit: " << near_billboard->index << "\n" << std::endl;
				}
				else if (count_cocos == 1) {//Si hi ha 1 coco col.loquem el coco 1

					Audiomanager::instance->PlaySound("data/sounds/shaking_palmera.wav", false);//Audio shaking palmera

					buttons_GUI.push_back(buttons[1]);
					std::cout << "\nCoco num 1 afegit: " << near_billboard->index << "\n" << std::endl;
				}
				else {//Si ja hi ha dos cocos printem error ja que no hauria d'haver entrat ja que els dos plans de cocos haurien d'estar utilitzats
					std::cout << "\nError palmera: " << near_index << "\n" << std::endl;
				}

				World::instance->billboards[near_index]->used = true;//Si agafem l'objecte, el pla ja ha estat utilitzat
			}

		}

		else if (near_billboard->index == 1) {//si el pla proper es una bottle

			if (buttons[12]->doButton() == true) {//Si el boto GRAB es clica

				grab_bottle(near_billboard->index);
				near_billboard->setTexture("data/island/quads/bottle_2.tga");
				World::instance->billboards[near_index]->used = true;//Si agafem l'objecte, el pla ja ha estat utilitzat
			}
		}

		else if (near_billboard->index == 2) {//si el pla proper es el cofre

			if (play_minigame2 == false) {
				if (buttons[15]->doButton() == false) {//Si el boto OPEN no es clica
					return;
				}
			}
			open_chest(near_index);

		}

		else if (near_billboard->index == 4) {//si el pla proper es la tenda

			int keys = 0;
			for (int i = 0; i < buttons_GUI.size(); i++) {//Recorrem el vector de buttons_GUI per veure si hi ha alguna clau (hi ha d'haver 2)

				if (buttons_GUI[i]->id == 9 || buttons_GUI[i]->id == 10) {
					//si ja hi ha alguna de les 2 claus fem contador ++
					keys++;
				}
			}
			//El open no apareixera si no te les dues claus 
			if (keys != 2) { //Si no hi ha les dues claus es pinta el candado i no passara res
				buttons[21]->center = Vector2(window_width * 0.5, window_height * 0.5);
				bool nothing = buttons[21]->doButton();
				return;
			}

			open_tent(near_index);
		}

		else if (near_billboard->index == 5) {//si el pla proper al muelle

			int rod = 0;
			for (int i = 0; i < buttons_GUI.size(); i++) {//Recorrem el vector de buttons_GUI per veure si hi ha la canya

				if (buttons_GUI[i]->id == 11) {
					rod++;
				}
			}
			if (rod != 1) {//Si no hi ha canya no es pot pescar
				return;
			}
			//Nomes apareix el boto fish si te la canya de pescar
			if (buttons[14]->doButton() == true) {//Si el boto FISH es clica

				fish(near_index);
			}

		}
	}

	draw_test_GUI(world->billboards, near_index);//Dibuixa tots els botons de la gui i si es cliquen i coincideix que estem aprop del pla on es pot realitzar la accio,
											//que es faci.

}
void Gameplay::render_environment(Camera* camera)
{
	//Creation of the sky

	current_shader = Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs");
	current_shader->enable();

	glDisable(GL_DEPTH_TEST);

	sky->setPositiontoZero();
	sky->m.translate(camera->eye.x, camera->eye.y, camera->eye.z);
	sky->render(camera);

	glEnable(GL_DEPTH_TEST);

	//Paiting the clouds

	//World::instance->cloud->render(camera);

	current_shader->disable();
	m.setIdentity();

	//Painting the floor of the island

	current_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	if (current_shader)
	{
		//enable shader
		current_shader->enable();

		//upload uniforms
		current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		//current_shader->setUniform("water_texture", Texture::Get("data/island/color_deep.tga"));
		current_shader->setUniform("water_texture", Texture::Get("data/island/color_deep.tga"));
		current_shader->setUniform("sand_texture", Texture::Get("data/island/sand.tga"));
		current_shader->setUniform("texture_mask", Texture::Get("data/island/island_mask.tga"));
		current_shader->setUniform("u_time", time);
		current_shader->setUniform("u_model", m);

		floormesh->render(GL_TRIANGLES);

		//disable shader
		current_shader->disable();
	}

}

void Gameplay::create_sea(Camera* camera) {

	//creating water
	m.setIdentity();
	m.translate(camera->eye.x, 12, camera->eye.z);
	Shader* current_shader = Shader::Get("data/shaders/entity_basic.vs", "data/shaders/water_sea.fs");
	current_shader->enable();

	//upload uniforms
	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	current_shader->setUniform("u_color", Vector4(1, 1, 1, 0.5));
	current_shader->setUniform("u_texture", Texture::Get("data/island/agua.tga"));
	current_shader->setUniform("u_time", time);
	current_shader->setUniform("u_model", m);
	current_shader->setUniform("u_cam_pos", camera->eye);
	current_shader->setUniform("u_sky_texture", Texture::Get("data/cielo.tga"));
	current_shader->setUniform("u_normal_water", Texture::Get("data/island/normal_map_water.tga"));

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	water_plane->render(GL_TRIANGLES);
	glDisable(GL_BLEND);

	current_shader->disable();
}


void Gameplay::resizeButtons(int width) {

	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->center.x = width + buttons[i]->center.x;
	}

}
void Gameplay::draw_test_GUI(std::vector<Billboard*> billboards, int index_billboard) {

	for (int i = 0; i < buttons_GUI.size(); i++) {//Recorrem per printar tots els botons de la GUI

		if (buttons_GUI[i]->used == true) {//Si el boto ha estat utilitzat no el mostrem i passarem a la seguent iteracio del loop
			continue;
		}

		if (buttons_GUI[i]->doButton() == true) {//Si s'ha clicat el botó, mirem quin botó és i farà unes coses o unes altres

			int id_button = buttons_GUI[i]->id;//Guardem el id del boto

			if (id_button == 0 || id_button == 1) {//Si hem clicat un coco, apareixera a la pantalla gran i es tallara amb el ganivet

				std::cout << "\n coco tocat \n" << std::endl;
				buttons[20]->center = Vector2(window_width * 0.5, window_height * 0.5);
				bool nothing = buttons[20]->doButton();
				last_id_touched = id_button;

			}
			else if (id_button == 2) {//Si he seleccionat el ganivet i l'ultim tocat es el coco, desapareixen els dos i apareix la clau

				if (last_id_touched == 0 || last_id_touched == 1) {
					break_coco(last_id_touched);
				}
				last_id_touched = id_button;
				delete_knife();//Un cop els dos cocos estan tallats, desapareix el ganivet

			}
			else if (id_button == 6 || id_button == 7 || id_button == 8) {//Si hem clicat una carta, les podrem llegir

				//Es mostra per pantalla la carta que s'ha clicat (carta 1: boto 17, carta 2: boto 18, carta 3: boto 19)
				buttons[id_button + 11]->center = Vector2(window_width * 0.5, window_height * 0.5);

				//TODO: Que nomes es faci un cop
				//Audiomanager::instance->PlaySound("data/sounds/opening_letter.wav", false);//Soroll d'obrir una carta

				bool nothing = buttons[id_button + 11]->doButton();
				last_id_touched = id_button;

			}

			if (index_billboard != -1) {//En el cas de que es vulgui interactuar amb el pla ens interessara saber que l'index_billboard es diferent a -1 que voldra dir que Si que hi ha un pla proper al character

				if (billboards[index_billboard]->index == 3 && (id_button == 3 || id_button == 4 || id_button == 5)) {//Si l'usuari esta aprop de la paret (Es podran col.locar petxines)

					bool nothing = buttons[id_button]->doButton();
					last_id_touched = id_button;

					place_shell();

					buttons_GUI[i]->used = true;//Eliminem la petxina que hem utilitzat
				}
			}

		}
	}

}

void Gameplay::initButtons()
{
	//Aquí es creen tots els botons
	Texture* texture_atlas = Texture::Get("data/island/GUI/atlas_1.tga");

	//Cocos: (hi haura 2)
	Button* aux_button = new Button(Vector2(350, 50), Vector4(0, 0, 128, 128), 0, texture_atlas, 2048);
	buttons.push_back(aux_button);//0
	aux_button = new Button(Vector2(450, 50), Vector4(0, 0, 128, 128), 1, texture_atlas, 2048);
	buttons.push_back(aux_button);//1

	//Ganivet:
	aux_button = new Button(Vector2(250, 50), Vector4(128, 0, 256, 128), 2, texture_atlas, 2048);
	buttons.push_back(aux_button);//2

	//Petxines: (hi haura 3)
	aux_button = new Button(Vector2(50, 50), Vector4(256, 0, 384, 128), 3, texture_atlas, 2048);
	buttons.push_back(aux_button);//3
	aux_button = new Button(Vector2(150, 50), Vector4(256, 0, 384, 128), 4, texture_atlas, 2048);
	buttons.push_back(aux_button);//4
	aux_button = new Button(Vector2(250, 50), Vector4(256, 0, 384, 128), 5, texture_atlas, 2048);
	buttons.push_back(aux_button);//5

	//Cartes: (hi haura 3 pero son diferents)
	aux_button = new Button(Vector2(50, 50), Vector4(384, 0, 512, 128), 6, texture_atlas, 2048);
	buttons.push_back(aux_button);//6
	aux_button = new Button(Vector2(150, 50), Vector4(512, 0, 640, 128), 7, texture_atlas, 2048);
	buttons.push_back(aux_button);//7
	aux_button = new Button(Vector2(250, 50), Vector4(640, 0, 768, 128), 8, texture_atlas, 2048);
	buttons.push_back(aux_button);//8

	//Claus: (hi haura 2 pero son diferents)
	aux_button = new Button(Vector2(350, 50), Vector4(768, 0, 896, 128), 9, texture_atlas, 2048);
	buttons.push_back(aux_button);//9 --> correspon al coco 0
	aux_button = new Button(Vector2(450, 50), Vector4(896, 0, 1024, 128), 10, texture_atlas, 2048);
	buttons.push_back(aux_button);//10 --> correspon al coco 1

	//Canya de pescar:
	aux_button = new Button(Vector2(350, 50), Vector4(1024, 0, 1152, 128), 11, texture_atlas, 2048);
	buttons.push_back(aux_button);//11

	//Agafar:
	aux_button = new Button(Vector2(50, 150), Vector4(1152, 0, 1280, 128), 12, texture_atlas, 2048);
	buttons.push_back(aux_button);//12

	//Shake:
	aux_button = new Button(Vector2(50, 150), Vector4(1280, 0, 1408, 128), 13, texture_atlas, 2048);
	buttons.push_back(aux_button);//13

	//Fish:
	aux_button = new Button(Vector2(50, 150), Vector4(1408, 0, 1536, 128), 14, texture_atlas, 2048);
	buttons.push_back(aux_button);//14

	//Open:
	aux_button = new Button(Vector2(50, 150), Vector4(1536, 0, 1664, 128), 15, texture_atlas, 2048);
	buttons.push_back(aux_button);//15

	//Ship:
	aux_button = new Button(Vector2(50, 50), Vector4(1664, 0, 1792, 128), 16, texture_atlas, 2048);
	buttons.push_back(aux_button);//15

	//CARTA 1:
	aux_button = new Button(Vector2(window_width * 0.5, window_height * 0.5), Vector4(0, 0, 640, 640), 17, Texture::Get("data/island/GUI/carta_1.tga"), 640);
	aux_button->height = 500;
	aux_button->width = 500;
	buttons.push_back(aux_button);//17

	//CARTA 2:
	aux_button = new Button(Vector2(window_width * 0.5, window_height * 0.5), Vector4(0, 0, 500, 500), 18, Texture::Get("data/island/GUI/carta_2.tga"), 500);
	aux_button->height = 500;
	aux_button->width = 500;
	buttons.push_back(aux_button);//18

	//CARTA 3:
	aux_button = new Button(Vector2(window_width * 0.5, window_height * 0.5), Vector4(0, 0, 500, 500), 19, Texture::Get("data/island/GUI/carta_3.tga"), 500);
	aux_button->height = 500;
	aux_button->width = 500;
	buttons.push_back(aux_button);//19

	//TODO Canviar COCO gran
	aux_button = new Button(Vector2(window_width * 0.5, window_height * 0.5), Vector4(0, 0, 128, 128), 20, texture_atlas, 2048);
	aux_button->height = 500;
	aux_button->width = 500;
	buttons.push_back(aux_button);//20

	//CERRADURA:
	aux_button = new Button(Vector2(window_width * 0.5, window_height * 0.5), Vector4(0, 0, 512, 512), 21, Texture::Get("data/island/GUI/keyhole.tga"), 500);
	aux_button->height = 500;
	aux_button->width = 500;
	buttons.push_back(aux_button);//19

}
//Gameplay functions
void Gameplay::grab_bottle(int near_index) {

	int bottles = 0;

	for (int i = 0; i < buttons_GUI.size(); i++) {//Recorrem el vector de buttons_GUI per veure si hi ha algun coco (hi poden haver 2)

		if (buttons_GUI[i]->id == 6 || buttons_GUI[i]->id == 7 || buttons_GUI[i]->id == 8) {
			//si ja hi ha alguna de les 3 botelles fem contador ++
			bottles++;
		}
	}
	if (bottles == 0) {//Si no hi ha cap botella col.loquem la primera
		buttons_GUI.push_back(buttons[6]);
		std::cout << "\nBotella num 6 afegida: " << near_index << "\n" << std::endl;
	}
	else if (bottles == 1) {//Si hi ha 1 botella col.loquem la segona
		buttons_GUI.push_back(buttons[7]);
		std::cout << "\nBotella num 7 afegida: " << near_index << "\n" << std::endl;
	}
	else if (bottles == 2) {//Si hi ha 2 botelles s'afegeix la tercera
		buttons_GUI.push_back(buttons[8]);
		std::cout << "\nBotella num 8 afegida: " << near_index << "\n" << std::endl;
	}
	else {//Si ja hi ha tre botelles printem error ja que no hauria d'haver entrat ja que els tres plans de botelles haurien d'estar utilitzats
		std::cout << "\nError palmera: " << near_index << "\n" << std::endl;
	}

}
void Gameplay::break_coco(int id_button) {

	for (int i = 0; i < buttons_GUI.size(); i++) {//Recorrem buttons GUI

		int aux_id = buttons_GUI[i]->id;

		if (id_button == aux_id) {

			Audiomanager::instance->PlaySound("data/sounds/obrint_coco.wav", false);//Audio trencant coco

			buttons_GUI[i]->used = true;//Coco utilitzat

			//afegim les claus que corresponen a cada coco
			//claus 9 i 10 relacionades amb cocos 0 i 1
			buttons_GUI.push_back(buttons[id_button + 9]);
		}

	}
}
void Gameplay::delete_knife() {

	int count_cocos = 0; //Per comprovar que l'usuari ha agafat els cocos
	bool can_delete = true;
	int index_ganivet;

	for (int i = 0; i < buttons_GUI.size(); i++) {

		if (buttons_GUI[i]->id == 0 || buttons_GUI[i]->id == 1) {
			count_cocos++;//Per assegurar-nos que l'usuari ha agafat tots els cocos
			if (buttons_GUI[i]->used == false) {//Si un dels cocos no ha estat utilitzat (encara es mostra per la GUI)
				can_delete = false;//No es podra borrar el ganivet
			}
		}
		else if (buttons_GUI[i]->id == 2) {
			index_ganivet = i;
		}
	}
	if (count_cocos == 2 && can_delete == true) {
		buttons_GUI[index_ganivet]->used = true;
	}

}
void Gameplay::place_shell()
{
	std::vector<Entity*> &entities = World::instance->entities;
	GLuint id;
	int index;
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->index == 3) {
			id = World::instance->entities[i]->getTextureId();
			index = i;
		}
	}

	//Canviem la textura
	if (id == 10) { //La textura encara no te cap petxina
		entities[index]->setTexture("data/island/meshes/wall_1_shell.tga");
	}
	if (id == 41) { //La textura te una petxina
		entities[index]->setTexture("data/island/meshes/wall_2_shells.tga");
	}
	if (id == 42) { //La textura te dues
		entities[index]->setTexture("data/island/meshes/wall_3_shells.tga");
		buttons_GUI.push_back(buttons[16]);//Afegim barco
	}

}
void Gameplay::open_chest(int near_index) {

	//Quan se li doni open:
	//S'obrira el minijoc (obrir el cofre)
	play_minigame2 = true;
	win_minigame2 = minigame->playMinigame2();
	if (win_minigame2 == -1) {
		//Es segueix jugant
		win_minigame2 = minigame->playMinigame2();
	}
	else if (win_minigame2 == 0) {
		//S'ha tancat pero no s'ha guanyat
		play_minigame2 = false;
	}
	else if (win_minigame2 == 1) {
		//Si playMinigame retorna 1, s'haura guanyat el minijoc i s'obtenen la petxina i la canya
		play_minigame2 = false;

		Audiomanager::instance->PlaySound("data/sounds/opening_chest.wav", false);//Soroll d'obrir un cofre

		//Es treuen les dos primeres cartes i es mou la tercera a la posicio de la primera
		for (int i = 0; i < buttons_GUI.size(); i++) {
			if (buttons_GUI[i]->id == 6 || buttons_GUI[i]->id == 7) {
				buttons_GUI[i]->used = true;
			}
			else if (buttons_GUI[i]->id == 8) {
				buttons_GUI[i]->center = Vector2(50, 50);
			}
		}

		buttons_GUI.push_back(buttons[2]);//Afegim ganivet
		buttons_GUI.push_back(buttons[4]);//Afegim petxina

		World::instance->billboards[near_index]->used = true;//Quan es guanyi ja no es podra tornar a utilitzar
	}
}
void Gameplay::open_tent(int near_index) {

	if (play_minigame1 == false) {
		if (buttons[15]->doButton() == false) {//Si el boto OPEN no es clica

			return;
		}
		else {
			//Perque es reprodueixi el so només una vegada
			Audiomanager::instance->PlaySound("data/sounds/candau.wav", false);//Audio obrint la tenda amb les claus
		}
	}

	//Quan se li doni open:
	for (int i = 0; i < buttons_GUI.size(); i++) {
		if (buttons_GUI[i]->id == 9 || buttons_GUI[i]->id == 10) {
			buttons_GUI[i]->used = true;
		}
	}

	//S'obrira el minijoc
	play_minigame1 = true;
	win_minigame1 = minigame->playMinigame1();
	if (win_minigame1 == -1) {
		//Es segueix jugant
		win_minigame1 = minigame->playMinigame1();
	}
	else if (win_minigame1 == 0) {
		//S'ha tancat pero no s'ha guanyat
		play_minigame1 = false;
	}
	else if (win_minigame1 == 1) {

		//Si playMinigame retorna 1, s'haura guanyat el minijoc i s'obtenen la petxina i la canya
		play_minigame1 = false;
		buttons_GUI.push_back(buttons[11]);//Afegim canya
		buttons_GUI.push_back(buttons[5]);//Afegim petxina

		//Es treu la carta que quedava
		for (int i = 0; i < buttons_GUI.size(); i++) {
			if (buttons_GUI[i]->id == 8) {
				buttons_GUI[i]->used = true;
			}
		}

		World::instance->billboards[near_index]->used = true;//Quan es guanyi ja no es podra tornar a utilitzar
	}
}
void Gameplay::fish(int near_index) {

	//Haurem de fer l'animacio de pescar
	buttons_GUI.push_back(buttons[3]);//Afegim petxina
	World::instance->billboards[near_index]->used = true;//Quan es pesqui ja no es podra tornar a utilitzar

	for (int i = 0; i < buttons_GUI.size(); i++) {
		if (buttons_GUI[i]->id == 11) {
			buttons_GUI[i]->used = true;//Eliminem la canya ja que ja l'hem utilitzat
		}
	}
}

/**********/
/**BUTTON**/
/**********/
Button::Button(Vector2 center_, Vector4 uvs_, int id_, Texture* texture_, float image_size_) {

	center = center_;
	uvs = uvs_;
	id = id_;
	texture = texture_;

	width = 100;
	height = 100;
	image_size = image_size_;
	used = false;
}

bool Button::doButton() {

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, 0, Game::instance->window_height, 0, 10000);

	Mesh m;
	m.createQuadGUI(center.x, center.y, width, height, uvs.x, uvs.y, uvs.z, uvs.w, image_size, false);

	Matrix44 model;

	Shader* shader = Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs");
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", texture);
	m.render(GL_TRIANGLES);

	shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Vector2 mouse_pos = Game::instance->mouse_pos;

	if (mouse_pos.x > (center.x - width * 0.5) && mouse_pos.x < (center.x + width * 0.5) && mouse_pos.y >(Game::instance->window_height - center.y - height * 0.5) && mouse_pos.y < (Game::instance->window_height - center.y + height * 0.5)) {

		mouse_pos = Vector2(0, 0);
		return true;
	}

	return false;

}

/************/
/**MINIGAME**/
/************/
Minigame::Minigame()
{
	resize();
	int w = width / 4;
	int h = height / 4;

	//Creem el vector de la solució del minigame 1
	solution1[0] = false;
	solution1[1] = true;
	solution1[2] = false;
	solution1[3] = false;
	solution1[4] = true;
	solution1[5] = true;
	solution1[6] = false;
	solution1[7] = false;
	solution1[8] = true;
	solution1[9] = true;
	solution1[10] = false;
	solution1[11] = true;
	solution1[12] = true;
	solution1[13] = false;
	solution1[14] = true;
	solution1[15] = true;

	//Creem el vector inicial del jugador del minigame 1
	for (int i = 0; i < 16; i++)
	{
		player1[i] = false;
	}

	//Creem el vector de la solució del minigame 2
	solution2[0] = 3;
	solution2[1] = 4;
	solution2[2] = 5;

	//Creem el vector inicial del jugador del minigame 2
	player2[0] = 0;
	player2[1] = 0;
	player2[2] = 0;

}
void Minigame::resize() {
	width = Game::instance->window_width;
	height = Game::instance->window_height;

	int w = ((width / 4) + 1) & ~1;; //Arrodoneix al numero parell mes gran
	int h = ((height / 4) + 1) & ~1;

	//Es fa/refa el vector de posicions pels quads
	positions_quad.clear();
	for (int i = w / 2; i <= w / 2 * 7; i = i + w) { //Comencem per les columnes de l'equerra
		for (int j = h / 2 * 7; j >= h / 2; j = j - h) {//I per les files de baix
			//Ho fem en aquest ordre perque les posicions dels quad tenen el 0,0 abaix a l'esquerra
			positions_quad.push_back(Vector2(i, j));
		}
	}
	//Es fa/refa el vector de posicions pel mouse
	positions_mouse.clear();
	for (int i = 0; i <= (w * 3); i = i + w) {
		for (int j = 0; j <= (h * 3); j = j + h) {
			positions_mouse.push_back(Vector2(i, j));
		}
	}

	//Es fa/refa el vector de posicions dels quads del minigame 2
	positions_quad2[0] = Vector2(w, h * 2);
	positions_quad2[1] = Vector2(w * 2, h * 2);
	positions_quad2[2] = Vector2(w * 3, h * 2);
}
int Minigame::playMinigame1()
{
	//MINIGAME DE LA TENDA
	//La funció retornara 0 si es tanca sense guanyar, 1 si es guanya i -1 si es segueix jugant

	resize();
	int w = width / 4;
	int h = height / 4;
	Vector2 mouse_pos = Game::instance->mouse_pos;
	Mesh aux;

	//Si no s'ha clicat, el vector estara a false i es pintaran cercles
	//Quan es cliqui el boolea canviara
	for (int i = 0; i < positions_mouse.size(); i++)
	{
		if (mouse_pos.x > (width - 50) && mouse_pos.y < width && mouse_pos.y > 0 && mouse_pos.y < 38) {
			return 0;
		}
		if (mouse_pos.x > positions_mouse[i].x && mouse_pos.x < positions_mouse[i].x + w
			&& mouse_pos.y > positions_mouse[i].y && mouse_pos.y < positions_mouse[i].y + h) {

			mouse_pos = Vector2(0, 0);
			if (player1[i] == false) {
				player1[i] = true;
			}
			else {
				player1[i] = false;
			}
		}
	}
	Game::instance->mouse_pos = Vector2(0, 0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, 0, Game::instance->window_height, 0, 10000);

	//Pintem el background
	aux.createQuad(width / 2, height / 2, width, height, false);
	paintQuad("data/island/minigame/background.tga", aux, cam2D);
	//Pintem la creu per tancar
	aux.createQuad(width - 25, height - 19, 64, 64, false);
	paintQuad("data/island/minigame/close.tga", aux, cam2D);

	//Depenent de si es true o false, pintem una cosa o una altra
	for (int i = 0; i < positions_quad.size(); i++)
	{
		if (player1[i] == false)
		{
			aux.createQuad(positions_quad[i].x, positions_quad[i].y, w / 2, h / 2, false);
			paintQuad("data/island/minigame/circle.tga", aux, cam2D);
		}
		else
		{
			aux.createQuad(positions_quad[i].x, positions_quad[i].y, w / 2, h / 2, false);
			paintQuad("data/island/minigame/cross.tga", aux, cam2D);
		}
	}
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	for (int i = 0; i < 16; i++)
	{
		if (player1[i] != solution1[i]) {
			return -1;
		}
	}
	return true;
}
int Minigame::playMinigame2()
{
	//MINIGAME DEL COFRE

	//La funció retornara 0 si es tanca sense guanyar, 1 si es guanya i -1 si es segueix jugant

	resize();
	int w = width / 4;
	int h = height / 4;
	Vector2 mouse_pos = Game::instance->mouse_pos;
	Mesh aux;

	//Si no s'ha clicat res, comencem amb 3 numeros 0
	//Quan es s'afegira un numero
	for (int i = 0; i < positions_mouse.size(); i++)
	{
		if (mouse_pos.x > (width - 50) && mouse_pos.y < width && mouse_pos.y > 0 && mouse_pos.y < 38) {
			return 0; //S'ha tancat el minigame
		}
		if (mouse_pos.x > w / 2 && mouse_pos.x < w / 2 * 3 && mouse_pos.y > h && mouse_pos.y < h / 2 * 6) { //El primer numero

			mouse_pos = Vector2(0, 0);
			if (player2[0] == 5) {
				player2[0] = 0;
			}
			else {
				player2[0] = player2[0] + 1;
			}
		}
		if (mouse_pos.x > w / 2 * 3 && mouse_pos.x < w / 2 * 5 && mouse_pos.y > h && mouse_pos.y < h / 2 * 6) { //El segon numero

			mouse_pos = Vector2(0, 0);
			if (player2[1] == 5) {
				player2[1] = 0;
			}
			else {
				player2[1] = player2[1] + 1;
			}
		}
		if (mouse_pos.x > w / 2 * 5 && mouse_pos.x < w / 2 * 7 && mouse_pos.y > h && mouse_pos.y < h * 6) { //El tercer numero

			mouse_pos = Vector2(0, 0);
			if (player2[2] == 5) {
				player2[2] = 0;
			}
			else {
				player2[2] = player2[2] + 1;
			}
		}
	}
	Game::instance->mouse_pos = Vector2(0, 0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, 0, Game::instance->window_height, 0, 10000);

	//Pintem el background
	aux.createQuad(width / 2, height / 2, width, height, false);
	paintQuad("data/island/minigame/background.tga", aux, cam2D);
	//Pintem la creu per tancar
	aux.createQuad(width - 25, height - 19, 64, 64, false);
	paintQuad("data/island/minigame/close.tga", aux, cam2D);

	//Depenent del numero que sigui, el pintem
	for (int i = 0; i < 3; i++)
	{
		if (player2[i] == 0) {
			aux.createQuad(positions_quad2[i].x, positions_quad2[i].y, w, h, false);
			paintQuad("data/island/minigame/zero.tga", aux, cam2D);

		}
		else if (player2[i] == 1) {
			aux.createQuad(positions_quad2[i].x, positions_quad2[i].y, w, h, false);
			paintQuad("data/island/minigame/one.tga", aux, cam2D);
		}
		else if (player2[i] == 2) {
			aux.createQuad(positions_quad2[i].x, positions_quad2[i].y, w, h, false);
			paintQuad("data/island/minigame/two.tga", aux, cam2D);
		}
		else if (player2[i] == 3) {
			aux.createQuad(positions_quad2[i].x, positions_quad2[i].y, w, h, false);
			paintQuad("data/island/minigame/three.tga", aux, cam2D);
		}
		else if (player2[i] == 4) {
			aux.createQuad(positions_quad2[i].x, positions_quad2[i].y, w, h, false);
			paintQuad("data/island/minigame/four.tga", aux, cam2D);
		}
		else if (player2[i] == 5) {
			aux.createQuad(positions_quad2[i].x, positions_quad2[i].y, w, h, false);
			paintQuad("data/island/minigame/five.tga", aux, cam2D);
		}
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	for (int i = 0; i < 3; i++)
	{
		if (player2[i] != solution2[i]) {
			return -1;
		}
	}return 1;
}
void Minigame::paintQuad(const char* texture, Mesh mesh, Camera cam2D) {

	Matrix44 model;

	Shader* shader = Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs");
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", Texture::Get(texture));
	mesh.render(GL_TRIANGLES);
	shader->disable();
}