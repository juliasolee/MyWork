#include "application.h"
#include "utils.h"
#include "image.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "light.h"
#include "material.h"
#include "texture.h"

Camera* camera = NULL;
Mesh* mesh = NULL;

Shader* phong = NULL;
Shader* goureud = NULL;
Shader* texture_shader = NULL;
Shader* current_shader = NULL;

Light* light = NULL;
Light* light2 = NULL;

Matrix44 model_matrix; //defines where is the object

Material* material = NULL;
Material* plasticMaterial;
Material* goldMaterial;
Material* silverMaterial;
Material* rubberMaterial;
Material* textureMaterial; //Material neutre per les textures

Texture* texture_color;
Texture* texture_normal;
Texture* current_texture = NULL;

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(NULL);
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;

	//here we create a global camera and set a position and projection properties
	camera = new Camera();
	camera->lookAt(Vector3(0, 20, 20), Vector3(0, 10, 0), Vector3(0, 1, 0));
	camera->setPerspective(60, window_width / window_height, 0.1, 10000);

	//then we load a mesh
	mesh = new Mesh();
	mesh->loadOBJ("data/lee.obj");

	//we load a shader
	phong = new Shader();
	goureud = new Shader();
	texture_shader = new Shader();
	current_shader = phong;

	phong->load("data/phong.vs", "data/phong.ps");
	goureud->load("data/goureud.vs", "data/goureud.ps");
	texture_shader->load("data/texture.vs", "data/texture.ps");


	//CODE HERE:
	//create a light (or several) and a material
	light = new Light();
	light2 = new Light();
	light2->setLight2_off();

	plasticMaterial = new Material();
	goldMaterial = new Material();
	silverMaterial = new Material();
	rubberMaterial = new Material();
	textureMaterial = new Material();
	material = plasticMaterial;

	plasticMaterial->setMaterial(Vector3(1, 1, 1), Vector3(0.5, 0.5, 0.5), Vector3(0.7, 0.7, 0.7), 32.0);
	goldMaterial->setMaterial(Vector3(0.24725, 0.1995, 0.0745), Vector3(0.75164, 0.60648, 0.22648), Vector3(0.628281, 0.555802, 0.366065), 51.2);
	silverMaterial->setMaterial(Vector3(0.23125, 0.23125, 0.23125), Vector3(0.2775, 0.2775, 0.2775), Vector3(0.773911, 0.773911, 0.773911), 89.6);
	rubberMaterial->setMaterial(Vector3(0.05, 0, 0), Vector3(0.5, 0.4, 0.4), Vector3(0.7, 0.04, 0.04), 10.0);
	textureMaterial->setMaterial(Vector3(1.0, 1.0, 1.0), Vector3(1.0, 1.0, 1.0), Vector3(1.0, 1.0, 1.0), 20.0);

	//load the texture
	texture_color = new Texture();
	if (!texture_color->load("data/lee_color_specular.tga"))
	{
		std::cout << "Texture not found" << std::endl;
		exit(1);
	}
	texture_normal = new Texture();
	if (!texture_normal->load("data/lee_normal.tga"))
	{
		std::cout << "Texture not found" << std::endl;
		exit(1);
	}

}



//render one frame
void Application::render(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//Get the viewprojection
	Matrix44 viewprojection = camera->getViewProjectionMatrix();
	Matrix44 mvp = model_matrix * viewprojection;

	//choose a shader entre phong i goureud


	//enable the shader
	current_shader->enable();
	current_shader->setMatrix44("model", model_matrix); //upload info to the shader
	current_shader->setMatrix44("mvp", mvp); //upload modelviewprojection info to the shader
	current_shader->setUniform3("Eye", camera->eye);
	light->uploadToShader(current_shader);
	light2->uploadToShader2(current_shader);
	material->uploadToShader(current_shader);
	current_shader->setTexture("color_texture", texture_color);
	current_shader->setTexture("normal_texture", texture_normal);

	
	//render the data
	mesh->render(GL_TRIANGLES);

	//disable shader
	current_shader->disable();

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

//called after render
void Application::update(double seconds_elapsed)
{
	if (keystate[SDL_SCANCODE_SPACE])
	{
		model_matrix.rotateLocal(seconds_elapsed, Vector3(0, 1, 0));
	}
	if (keystate[SDL_SCANCODE_A])
		//Movem el vector eye (la camara) a la posicio de les x negatives
		camera->eye.x -= 15 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_D])

		//Movem el vector eye (la camara) a la posicio de les x positives
		camera->eye.x += 15 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_W])
		//Movem el vector eye (la camara) a la posicio de les y positives
		camera->eye.y += 15 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_S])
		//Movem el vector eye (la camara) a la posicio de les y negatives
		camera->eye.y -= 15 * seconds_elapsed;

	//Per moure la llum amb les fletxes del teclat
	if (keystate[SDL_SCANCODE_LEFT] && !(keystate[SDL_SCANCODE_L]))
		//Movem l'il.luminacio en la posicio negatives de les x
		light->position.x -= 15 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_RIGHT] && !(keystate[SDL_SCANCODE_L]))
		//Movem l'il.luminacio en la posicio positives de les x
		light->position.x += 15 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_UP] && !(keystate[SDL_SCANCODE_L]) && !(keystate[SDL_SCANCODE_Z]) && !(keystate[SDL_SCANCODE_X]) && !(keystate[SDL_SCANCODE_C]))
		//Movem l'il.luminacio en la posicio positives de les y
		light->position.y += 15 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_DOWN] && !(keystate[SDL_SCANCODE_L]) && !(keystate[SDL_SCANCODE_Z]) && !(keystate[SDL_SCANCODE_X]) && !(keystate[SDL_SCANCODE_C]))
		//Movem l'il.luminacio en la posicio negatives de les y
		light->position.y -= 15 * seconds_elapsed;


	if ((keystate[SDL_SCANCODE_Z]) && (keystate[SDL_SCANCODE_UP])) {
		light->ambient.x = light->ambient.x + 0.1 * seconds_elapsed;
		light->ambient.y = light->ambient.y + 0.1 * seconds_elapsed;
		light->ambient.z = light->ambient.z + 0.1 * seconds_elapsed;
	}
	if ((keystate[SDL_SCANCODE_Z]) && (keystate[SDL_SCANCODE_DOWN])) {
		light->ambient.x = light->ambient.x - 0.1 * seconds_elapsed;
		light->ambient.y = light->ambient.y - 0.1 * seconds_elapsed;
		light->ambient.z = light->ambient.z - 0.1 * seconds_elapsed;
	}
	if ((keystate[SDL_SCANCODE_X]) && (keystate[SDL_SCANCODE_UP])) {
		light->diffuse_color.x = light->diffuse_color.x + 0.1 * seconds_elapsed;
		light->diffuse_color.y = light->diffuse_color.y + 0.1 * seconds_elapsed;
		light->diffuse_color.z = light->diffuse_color.z + 0.1 * seconds_elapsed;
	}
	if ((keystate[SDL_SCANCODE_X]) && (keystate[SDL_SCANCODE_DOWN])) {
		light->diffuse_color.x = light->diffuse_color.x - 0.1 * seconds_elapsed;
		light->diffuse_color.y = light->diffuse_color.y - 0.1 * seconds_elapsed;
		light->diffuse_color.z = light->diffuse_color.z - 0.1 * seconds_elapsed;
	}
	if ((keystate[SDL_SCANCODE_C]) && (keystate[SDL_SCANCODE_UP])) {
		light->specular_color.x = light->specular_color.x + 0.1 * seconds_elapsed;
		light->specular_color.y = light->specular_color.y + 0.1 * seconds_elapsed;
		light->specular_color.z = light->specular_color.z + 0.1 * seconds_elapsed;
	}
	if ((keystate[SDL_SCANCODE_C]) && (keystate[SDL_SCANCODE_DOWN])) {
		light->specular_color.x = light->specular_color.x - 0.1 * seconds_elapsed;
		light->specular_color.y = light->specular_color.y - 0.1 * seconds_elapsed;
		light->specular_color.z = light->specular_color.z - 0.1 * seconds_elapsed;
	}


	//Movem el segon llum: es moura mentre apretem l i les fletxes
	if ((keystate[SDL_SCANCODE_L]) && (keystate[SDL_SCANCODE_LEFT])) {
		//Movem l'il.luminacio en la posicio negatives de les x
		light2->position.x -= 15 * seconds_elapsed;
	}
	if ((keystate[SDL_SCANCODE_L]) && (keystate[SDL_SCANCODE_RIGHT])) {
		//Movem l'il.luminacio en la posicio positives de les x
		light2->position.x += 15 * seconds_elapsed;
	}
	if ((keystate[SDL_SCANCODE_L]) && (keystate[SDL_SCANCODE_UP])) {
		//Movem l'il.luminacio en la posicio positives de les y
		light2->position.y += 15 * seconds_elapsed;
	}
	if ((keystate[SDL_SCANCODE_L]) && (keystate[SDL_SCANCODE_DOWN])) {
		//Movem l'il.luminacio en la posicio negatives de les y
		light2->position.y -= 15 * seconds_elapsed;

	}

}


//keyboard press event 
void Application::onKeyPressed(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE:
		exit(0);
		break; //ESC key, kill the app
	case SDLK_PLUS://Ens apropem a la figura. Fem mes petit el fov de la camara
		camera->fov--;
		break;
	case SDLK_MINUS://Ens allunyem de la figura. Fem mes gran el fov de la camara
		camera->fov++;
		break;
	case SDLK_p:
		//Carreguem el shader de phong
		current_shader = phong;
		break;
	case SDLK_g:
		//Carreguem el shader de goureud
		current_shader = goureud;
		break;
	case SDLK_1:
		//Material = plastic
		current_shader = phong;
		material = plasticMaterial;
		break;
	case SDLK_2:
		//Material = gold
		current_shader = phong;
		material = goldMaterial;
		break;
	case SDLK_3:
		//Material = silver
		current_shader = phong;
		material = silverMaterial;
		break;
	case SDLK_4:
		//Material = rubber
		current_shader = phong;
		material = rubberMaterial;
		break;
	case SDLK_5:
		//Textura1 (color)
		material = textureMaterial;
		light->reset();
		current_shader = texture_shader;
		break;
	case SDLK_7:
		//llum vermella
		light->setRed();
		break;
	case SDLK_8:
		//llum verda
		light->setGreen();
		break;
	case SDLK_9:
		//llum blava
		light->setBlue();
		break;
	case SDLK_0:
		//llum default
		light->reset();
		break;
	case SDLK_h:
		//Encenem la segona llum
		light2->setLight2_on();
		break;
	case SDLK_o:
		//Encenem la segona llum
		light2->setLight2_off();
		break;
	}
}
//keyboard key up event 

//mouse button event
void Application::onMouseButtonDown(SDL_MouseButtonEvent event)
{

	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
	}
}

void Application::onMouseButtonUp(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{

	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}















 