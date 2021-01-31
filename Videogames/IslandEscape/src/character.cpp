#include "character.h"
#include "shader.h"
#include "camera.h"
#include "input.h"
#include "game.h"
#include "scene.h"

Character::Character(Mesh *mesh_, Texture *texture_)
{
	mesh = mesh_;
	texture = texture_;

	pos = Vector3(500, 20.0,500);//Character es situa a l'inici del room escape
	yaw = -90.0;
	pitch = 0.0;
	cam_yaw = 0.0;
}

void Character::updateMatrix()
{
	model.setIdentity();
	model.translate(pos.x, pos.y, pos.z);
	model.rotate((yaw * M_PI / 180), Vector3(0.0, 1.0, 0.0));
	model.scale(0.3, 0.3, 0.3);

}

void Character::render(Camera* camera, Shader* shader)
{

	if (!shader)
		return;

	shader->enable();

	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("texture_entity", texture);
	shader->setUniform("u_time", 0.0f);
	shader->setUniform("u_model", model);
	shader->setUniform("u_color", Vector4(1,1,1,1));

	mesh->renderAnimated(GL_TRIANGLES, &sk);

	shader->disable();

}

void Character::update(Camera* camera, double seconds_elapsed)
{
	World* instance = World::instance; //Copy of the instance of the world

	Vector3 prev_pos = pos; //Saving previous position in case of collision
	float speed = seconds_elapsed * 15; //the speed is defined by the seconds_elapsed so it goes constant
	float rot_speed = seconds_elapsed * 5; //the speed of the rotation
	Vector3 move;

	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) || Input::isKeyPressed(SDL_SCANCODE_RSHIFT)) speed = speed * 5;
	if (Input::isKeyPressed(SDL_SCANCODE_W)) move.z += speed;
	if (Input::isKeyPressed(SDL_SCANCODE_S)) move.z -= speed;
	
	//mouse input to rotate the position
	if (Game::instance->mouse_locked || Input::isMousePressed(SDL_BUTTON_LEFT))
	{

		yaw -= Input::mouse_delta.x * (seconds_elapsed * 13);

	}

	//To simulate a more natural movement of the character we use velocity
	Matrix44 rot;
	rot.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));
	move = rot * move;
	velocity = velocity + move * 5;
	Vector3 aux_pos = pos + velocity * seconds_elapsed;
	
	//Color aux_col = instance->test_mask->getPixel(aux_pos.x, aux_pos.z);
	if (instance->test_mask->getPixel(aux_pos.x, aux_pos.z).x == 255) {//Podra caminar
		pos = pos + velocity * seconds_elapsed;
	}

	//Add friction to the movement
	float friction = 1.0 / (1.0 + (seconds_elapsed * 5));
	velocity = velocity * friction;

	testCharCollision(seconds_elapsed, prev_pos);

}

void Character::computeAnim(Camera* camera) {

	Matrix44 m_Local;
	m_Local.rotate(-yaw * DEG2RAD, Vector3(0, 1, 0));

	//Velocidad de coord globales a locales
	Vector3 vel_global = m_Local * velocity;

	Shader* current_shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/entity.fs");
	float t = Game::instance->time;
	float w;

	Animation *anim1, *anim2;

	if (vel_global.z >= 75) {//Running only
		
		anim1 = Animation::Get("data/character/running.skanim");
		anim1->assignTime(t);

		anim2 = Animation::Get("data/character/running.skanim");
		anim2->assignTime(t);

		w = clamp(vel_global.length(), 0, 1);
	}
	else if (vel_global.z >= 15 && vel_global.z < 75) {//Walking - running forward

		anim1 = Animation::Get("data/character/walking.skanim");
		anim1->assignTime(t);

		anim2 = Animation::Get("data/character/running.skanim");
		float time_b = (t / anim1->duration) * anim2->duration;
		anim2->assignTime(time_b);

		float b = (vel_global.z - 15) / 60;
		float a = 1 - b;

		w = clamp(vel_global.length(), a, b);
	}
	else if(vel_global.z < 15 && vel_global.z > 1){//idle - walking forward

		anim1 = Animation::Get("data/character/idle.skanim");
		anim1->assignTime(t);

		anim2 = Animation::Get("data/character/walking.skanim");
		anim2->assignTime(t);

		float b = (vel_global.z - 1) / 14;
		float a = 1 - b;

		w = clamp(vel_global.length(), a, b);
	}
	else if (vel_global.z < -1 && vel_global.z > -15) {//idle - walking backwards

		anim1 = Animation::Get("data/character/idle.skanim");
		anim1->assignTime(t);

		anim2 = Animation::Get("data/character/walking_backwards.skanim");
		anim2->assignTime(t);

		float b = (- vel_global.z - 1) / 14;
		float a = 1 - b;

		w = clamp(vel_global.length(), a, b);
	}
	else if (vel_global.z <= -15 && vel_global.z > -75) {//walking - running backwards

		anim1 = Animation::Get("data/character/walking_backwards.skanim");
		anim1->assignTime(t);

		anim2 = Animation::Get("data/character/running_backwards.skanim");
		float time_b = (t / anim1->duration) * anim2->duration;
		anim2->assignTime(time_b);

		float b = (- vel_global.z - 15) / 60;
		float a = 1 - b;

		w = clamp(vel_global.length(), a, b);
	}
	else if (vel_global.z <= -75) {//Running backwards only

		anim1 = Animation::Get("data/character/running_backwards.skanim");
		anim1->assignTime(t);

		anim2 = Animation::Get("data/character/running_backwards.skanim");
		anim2->assignTime(t);

		w = clamp(vel_global.length(), 0, 1);
	}
	else{//idle only

		anim1 = Animation::Get("data/character/idle.skanim");
		anim1->assignTime(t);

		anim2 = Animation::Get("data/character/idle.skanim");
		anim2->assignTime(t);

		w = clamp(vel_global.length(), 0, 1);
	}

	Skeleton blended_skeleton;
	blendSkeleton(&anim1->skeleton, &anim2->skeleton, w, &blended_skeleton);
	sk = blended_skeleton;

	render(camera, current_shader);

}

void Character::testCharCollision(float seconds_elapsed, Vector3 prev_pos) {

	if (prev_pos.distance(pos) > 0.001) {//if the character is moving (not static in the same place) we are going to check the collisions with static objects of the scene

		Vector3 ray_o = prev_pos;//Ray origin
		Vector3 ray_d = normalize(pos - prev_pos);//Ray direction 
		float dist = (pos - prev_pos).length();

		Vector3 col_point;//Collision point
		Vector3 col_normal;//normal vector in the collision point

		Vector3 char_center = pos + Vector3(0, 4, 0);//Center of the sphere envolving the character

		for (int i = 0; i < World::instance->entities.size(); i++) {

			Entity* ent = World::instance->entities[i]; //Assign a entity of the vector entities
			Mesh* mesh = ent->mesh; // the mesh of the entity
			
			//Sphere Collision
			if (mesh->testSphereCollision(ent->m, char_center, 2, col_point, col_normal) == false)
				continue;//if there is not collision, continue

			//Effect of pushing away from the collision
			Vector3 push_away = normalize(col_point - ray_o) * seconds_elapsed;
			pos = prev_pos - push_away;

			pos.y = 20;//As we are walking in 2D, the y coordinate is 0

			break;
		}

		for (int i = 0; i < World::instance->billboards.size(); i++) {

			Billboard* b = World::instance->billboards[i]; //Assign a entity of the vector entities
			Mesh* mesh = b->mesh; // the mesh of the entity

			//Sphere Collision
			if (mesh->testSphereCollision(b->m, char_center, 2, col_point, col_normal) == false)
				continue;//if there is not collision, continue

			//Effect of pushing away from the collision
			Vector3 push_away = normalize(col_point - ray_o) * seconds_elapsed;
			pos = prev_pos - push_away;

			pos.y = 20;//As we are walking in 2D, the y coordinate is 0

			break;
		}
	}



}

int Character::isNear(std::vector<Billboard*> billboards) {
	
	float dist_aux = 30;
	float close_dist;
	
	int near_billboard = -1;

	//Recorre tots els plans per comprovat si estan aprop
	for (int i = 0; i < billboards.size(); i++) {
		
		if (billboards[i]->index != -1) {
			
			Vector2 pos_billboard(billboards[i]->position.x * 2, billboards[i]->position.z * 2);
			Vector2 pos_player(pos.x, pos.z);
			float dist = pos_player.distance(pos_billboard);

			if (billboards[i]->index == 3 || billboards[i]->index == 5) {
				close_dist = 30;
			}
			else {
				close_dist = 10;
			}

			if (dist < close_dist) {
				if ((dist < dist_aux) && billboards[i]->used == false) {//Sempre agafa la dist mes petita
					dist_aux = dist;
					near_billboard = i;
				}
			}
		}
	}

	//Retornara la posicio del pla mes proper dins el vector de plans
	return near_billboard;
}
