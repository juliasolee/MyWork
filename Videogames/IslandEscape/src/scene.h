#ifndef SCENE_H
#define SCENE_H

#include "includes.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "character.h"
#include "animation.h"

#include <cmath>

class Entity
{

public:
	Entity(Mesh *mesh_, Texture *texture_, Shader *shader_, Matrix44 m_, int index_); //constructor

	//atributs

	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Matrix44 m;
	int index;

	//methods overwritten
	virtual void render(Camera *camera);

	//methods
	GLuint getTextureId();
	void setTexture(const char* texture_);
	Vector3 getPosition();
	void setPositiontoZero();
	Vector3 getDimensions();

};

class Billboard
{
public:
	Billboard(Texture *texture_, Shader *shader_, Matrix44 m_, int index_);

	Texture* texture;
	Shader* shader;
	Matrix44 m;
	Mesh* mesh;
	int index;
	bool used;

	Texture* texture_above;

	int width;
	int height;
	Vector3 position;

	void setTexture(const char* texture_);
	void render(Camera* camera);
	void setPositiontoZero();

};

class World
{
public:


	static World* instance;

	World();

	Character* player;

	std::vector<Entity*> entities;//Vector of entities
	std::vector<Billboard*> billboards;//Vector of billboards
	
	Image* mask;
	Image* test_mask;

	void createWorld(Camera* camera);
	void render(Camera* camera);
	void readPositions(const char* filename, Camera* camera);

};


#endif

