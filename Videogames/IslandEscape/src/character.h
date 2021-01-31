#ifndef CHARACTER_H
#define CHARACTER_H

#include "includes.h"
#include "mesh.h"
#include "texture.h"
#include "animation.h"

class Billboard;

class Character
{

public:
	Character(Mesh *mesh_, Texture *texture_);

	//atributs
	Vector3 pos;
	float yaw;
	float pitch;
	float cam_yaw;

	Matrix44 model;
	Vector3 velocity;

	Mesh* mesh;
	Texture* texture;

	Skeleton sk;
	std::vector<Matrix44> bone_matrices;

	void render(Camera* camera, Shader* shader);
	void updateMatrix();
	void update(Camera* camera, double seconds_elapsed);
	void computeAnim(Camera* camera);

	void testCharCollision(float seconds_elapsed, Vector3 prev_pos);
	int isNear(std::vector<Billboard*> billboards);

};


#endif