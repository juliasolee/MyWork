#include "scene.h"
#include "utils.h"
#include "mesh.h"
#include "camera.h"
#include "input.h"
#include "game.h"


#include <vector>
#include "framework.h"

#include <map>
#include <string>

/**********/
/**ENTITY**/
/**********/
Entity::Entity(Mesh *mesh_, Texture *texture_, Shader *shader_, Matrix44 m_, int index_)
{
	mesh = mesh_;
	texture = texture_;
	shader = shader_;
	m = m_;
	index = index_;

	//Iniciem la posicio de la matriu a zero
	setPositiontoZero();

}

GLuint Entity::getTextureId() {

	return texture->texture_id;
}

void Entity::setTexture(const char* texture_) {

	texture = Texture::Get(texture_);

}

void Entity::render(Camera* camera)
{
	if (!shader)
		return;

	BoundingBox box = transformBoundingBox(m, mesh->box);

	char test = camera->testBoxInFrustum(box.center, box.halfsize);
	if (test == CLIP_OUTSIDE)
		return;

	shader->enable();

	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("texture_entity", texture);
	shader->setUniform("u_time", 0.0f);
	shader->setUniform("u_model", m);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_color_water", Vector4(1, 1, 1, 0.5));

	mesh->render(GL_TRIANGLES);

	shader->disable();
}

Vector3 Entity::getPosition()
{
	//Torna un vector4 on x i y corresponen a x i z del origen del objecte dins del pla
	//i y i w corresponen a x i z del punt final de l'objecte dins del pla
	Vector3 position;
	position.x = m.getTranslation().x;
	position.y = m.getTranslation().y;
	position.z = m.getTranslation().z;

	return position;
}

void Entity::setPositiontoZero()
{
	m.translate(-(getPosition().x), -(getPosition().y), -(getPosition().z));
}

Vector3 Entity::getDimensions()
{
	Vector3 dimensions;
	dimensions.x = floor(mesh->box.halfsize.x * 2);
	dimensions.y = floor(mesh->box.halfsize.y * 2);
	dimensions.z = floor(mesh->box.halfsize.z * 2);
	return dimensions;
}

/**************/
/**BILLBOARDS**/
/**************/
Billboard::Billboard(Texture *texture_, Shader *shader_, Matrix44 m_, int index_)
{
	texture = texture_;
	shader = shader_;
	m = m_;
	mesh = new Mesh();
	index = index_;
	used = false;

	//Iniciem la posicio de la matriu a zero
	setPositiontoZero();
}
void Billboard::setTexture(const char* texture_) {
	texture = Texture::Get(texture_);
}
void Billboard::render(Camera* camera)
{

	if (!shader)
		return;

	Vector3 U = camera->getLocalVector(Vector3(0, 1, 0)) * (height / 2);
	Vector3 R = camera->getLocalVector(Vector3(1, 0, 0)) * (width / 2);

	mesh->createQuad_U_R(position, R, U);

	shader->enable();
	shader->setUniform("u_model", m);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));

	if (camera->up.x == 0 && camera->up.z == 0) {
		shader->setUniform("u_texture", texture);
	}
	else {
		shader->setUniform("u_texture", texture_above);
	}


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mesh->render(GL_TRIANGLES);

	shader->disable();
	glDisable(GL_BLEND);


}

void Billboard::setPositiontoZero()
{
	m.translate(-(position.x), -(position.y), -(position.z));
}

/**********/
/**WORLD**/
/**********/
World* World::instance = NULL;

World::World()
{
	//To assure that we are not creatinc two instances of world
	assert(instance == NULL);
	instance = this;//Saving the instance
	mask = new Image();
}

void World::createWorld(Camera* camera) {

	player = new Character(Mesh::Get("data/character/male.mesh"), Texture::Get("data/character/male.png"));
	readPositions("data/positions.txt", camera);
}

void World::render(Camera* camera)
{

	//We print all the entities
	for (int i = 0; i < entities.size(); i++) { //i starts in 1 because the entity in position 0 is the sky

		entities.at(i)->render(camera);
	}

	//We print all the billboards
	for (int i = 0; i < billboards.size(); i++) { //i starts in 1 because the entity in position 0 is the sky

		billboards[i]->render(camera);
	}

	//Animations of the players
	player->computeAnim(camera);
}

void World::readPositions(const char* filename, Camera* camera) {

	struct stat stbuffer;

	FILE* f = fopen(filename, "rb");
	if (f == NULL)
	{
		std::cerr << "File not found: " << filename << std::endl;
	}

	stat(filename, &stbuffer);

	unsigned int size = stbuffer.st_size;
	char* data = new char[size + 1];
	fread(data, size, 1, f);
	fclose(f);
	data[size] = 0;

	char* pos = data;
	char line[255];
	int i = 0;

	Matrix44 model;

	//parse file
	while (*pos != 0)
	{
		if (*pos == '\n') pos++;
		if (*pos == '\r') pos++;

		//read one line
		i = 0;
		while (i < 255 && pos[i] != '\n' && pos[i] != '\r' && pos[i] != 0) i++;
		memcpy(line, pos, i);
		line[i] = 0;
		pos = pos + i;

		//std::cout << "Line: \"" << line << "\"" << std::endl;
		if (*line == '#' || *line == 0) continue; //comment

		//tokenize line
		std::vector<std::string> tokens = tokenize(line, " ");

		if (tokens.empty()) continue;

		//CREATE THE ENTITIES
		if (tokens[0] == "m" && tokens.size() == 4) //estem creant meshes
		{
			if (tokens[1] == "1") //House
			{

				Vector3 pos;
				pos.parseFromText(tokens[2].c_str(), '/');

				Vector2 rot;
				rot.parseFromText(tokens[3].c_str());

				Entity* house = new Entity(Mesh::Get("data/island/meshes/house.OBJ"), Texture::Get("data/island/meshes/materials.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, -1);
				house->m.translate(pos.x, pos.y, pos.z);
				house->m.rotate(rot.x * DEG2RAD, Vector3(0, 1, 0));
				entities.push_back(house);

			}
			else if (tokens[1] == "2") //House's wall
			{

				Vector3 pos;
				pos.parseFromText(tokens[2].c_str(), '/');

				Vector2 rot;
				rot.parseFromText(tokens[3].c_str());

				Entity* wall = new Entity(Mesh::Get("data/island/meshes/wall.OBJ"), Texture::Get("data/island/meshes/wall_no_shells.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, 3);
				wall->m.translate(pos.x, pos.y, pos.z);
				wall->m.rotate(rot.x * DEG2RAD, Vector3(0, 1, 0));
				entities.push_back(wall);

				//Creem un pla al final del dock on no hi pintarem res pero aixi podrem interactuar amb el port
				Billboard* wall_q = new Billboard(Texture::Get("data/island/quads/dock.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, 3);

				wall_q->texture_above = Texture::Get("data/island/quads/dock.tga");

				wall_q->position.x = pos.x / 2 - 5;
				wall_q->position.y = pos.y / 2;
				wall_q->position.z = pos.z / 2;

				wall_q->width = 2;
				wall_q->height = 20;

				wall_q->m.translate(wall_q->position.x, wall_q->position.y, wall_q->position.z);
				billboards.push_back(wall_q);

			}
			else if (tokens[1] == "3") //Dock
			{

				Vector3 pos;
				pos.parseFromText(tokens[2].c_str(), '/');

				Vector2 rot;
				rot.parseFromText(tokens[3].c_str());

				Entity* dock = new Entity(Mesh::Get("data/island/meshes/dock.OBJ"), Texture::Get("data/island/meshes/dock.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, -1);
				dock->m.translate(pos.x, pos.y, pos.z);
				dock->m.rotate(rot.x * DEG2RAD, Vector3(0, 1, 0));
				entities.push_back(dock);

				//Creem un pla al final del dock on no hi pintarem res pero aixi podrem interactuar amb el port
				Billboard* dock_q = new Billboard(Texture::Get("data/island/quads/dock.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, 5);

				dock_q->texture_above = Texture::Get("data/island/quads/dock.tga");

				dock_q->position.x = pos.x / 2 - 5;
				dock_q->position.y = pos.y / 2 + 10;
				dock_q->position.z = pos.z / 2 + 20;

				dock_q->width = 20;
				dock_q->height = 20;

				dock_q->m.translate(dock_q->position.x, dock_q->position.y, dock_q->position.z);
				billboards.push_back(dock_q);

			}
		}
		else if (tokens[0] == "q" && tokens.size() == 4) //estem creant quads
		{
			if (tokens[1] == "1") { //Palm tree

				//Palm_trees
				Billboard* palm_tree = new Billboard(Texture::Get("data/island/quads/palm_tree.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, -1);

				palm_tree->texture_above = Texture::Get("data/island/quads/palm_tree_above.tga");

				Vector3 pos;
				pos.parseFromText(tokens[2].c_str(), '/');
				palm_tree->position.x = pos.x / 2;
				palm_tree->position.y = pos.y / 2;
				palm_tree->position.z = pos.z / 2;

				Vector3 size;
				size.parseFromText(tokens[3].c_str(), '/');
				palm_tree->width = size.x;
				palm_tree->height = size.y;

				palm_tree->m.translate(palm_tree->position.x, palm_tree->position.y, palm_tree->position.z);
				billboards.push_back(palm_tree);
			}
			else if (tokens[1] == "2") { //Palm tree coco

				//Palm_trees_coco
				Billboard* palm_tree_coco = new Billboard(Texture::Get("data/island/quads/palm_tree_cocos.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, 0);

				palm_tree_coco->texture_above = Texture::Get("data/island/quads/palm_tree_above.tga");

				Vector3 pos;
				pos.parseFromText(tokens[2].c_str(), '/');
				palm_tree_coco->position.x = pos.x / 2;
				palm_tree_coco->position.y = pos.y / 2;
				palm_tree_coco->position.z = pos.z / 2;

				Vector3 size;
				size.parseFromText(tokens[3].c_str(), '/');
				palm_tree_coco->width = size.x;
				palm_tree_coco->height = size.y;

				palm_tree_coco->m.translate(palm_tree_coco->position.x, palm_tree_coco->position.y, palm_tree_coco->position.z);
				billboards.push_back(palm_tree_coco);
			}
			else if (tokens[1] == "3") //Bottles
			{
				//Letter bottles
				Billboard* bottle = new Billboard(Texture::Get("data/island/quads/bottle_1.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, 1);

				bottle->texture_above = Texture::Get("data/island/quads/bottle_above.tga");

				Vector3 pos;
				pos.parseFromText(tokens[2].c_str(), '/');
				bottle->position.x = pos.x / 2;
				bottle->position.y = pos.y / 2;
				bottle->position.z = pos.z / 2;

				Vector3 size;
				size.parseFromText(tokens[3].c_str(), '/');
				bottle->width = size.x;
				bottle->height = size.y;

				bottle->m.translate(bottle->position.x, bottle->position.y, bottle->position.z);
				billboards.push_back(bottle);
			}
			else if (tokens[1] == "4") //Chest
			{
				//Letter bottles
				Billboard* chest = new Billboard(Texture::Get("data/island/quads/chest.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, 2);

				chest->texture_above = Texture::Get("data/island/quads/chest_above.tga");

				Vector3 pos;
				pos.parseFromText(tokens[2].c_str(), '/');
				chest->position.x = pos.x / 2;
				chest->position.y = pos.y / 2;
				chest->position.z = pos.z / 2;

				Vector3 size;
				size.parseFromText(tokens[3].c_str(), '/');
				chest->width = size.x;
				chest->height = size.y;

				chest->m.translate(chest->position.x, chest->position.y, chest->position.z);
				billboards.push_back(chest);
			}
			else if (tokens[1] == "5") //Tent
			{
				//Letter bottles
				Billboard* tent = new Billboard(Texture::Get("data/island/quads/tent.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, 4);

				tent->texture_above = Texture::Get("data/island/quads/tent_above.tga");

				Vector3 pos;
				pos.parseFromText(tokens[2].c_str(), '/');
				tent->position.x = pos.x / 2;
				tent->position.y = pos.y / 2;
				tent->position.z = pos.z / 2;

				Vector3 size;
				size.parseFromText(tokens[3].c_str(), '/');
				tent->width = size.x;
				tent->height = size.y;

				tent->m.translate(tent->position.x, tent->position.y, tent->position.z);
				billboards.push_back(tent);
			}
			else if (tokens[1] == "6") //Cartell
			{
				//Letter bottles
				Billboard* cartell = new Billboard(Texture::Get("data/island/quads/cartell.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, -1);

				cartell->texture_above = Texture::Get("data/island/quads/cartell_above.tga");

				Vector3 pos;
				pos.parseFromText(tokens[2].c_str(), '/');
				cartell->position.x = pos.x / 2;
				cartell->position.y = pos.y / 2;
				cartell->position.z = pos.z / 2;

				Vector3 size;
				size.parseFromText(tokens[3].c_str(), '/');
				cartell->width = size.x;
				cartell->height = size.y;

				cartell->m.translate(cartell->position.x, cartell->position.y, cartell->position.z);
				billboards.push_back(cartell);
			}
			else if (tokens[1] == "7") //Rocks
			{
				//Letter bottles
				Billboard* rock = new Billboard(Texture::Get("data/island/quads/rock.tga"), Shader::Get("data/shaders/entity_basic.vs", "data/shaders/entity.fs"), model, -1);

				rock->texture_above = Texture::Get("data/island/quads/rock_above.tga");

				Vector3 pos;
				pos.parseFromText(tokens[2].c_str(), '/');
				rock->position.x = pos.x / 2;
				rock->position.y = pos.y / 2;
				rock->position.z = pos.z / 2;

				Vector3 size;
				size.parseFromText(tokens[3].c_str(), '/');
				rock->width = size.x;
				rock->height = size.y;

				rock->m.translate(rock->position.x, rock->position.y, rock->position.z);
				billboards.push_back(rock);
			}
		}
	}
}


