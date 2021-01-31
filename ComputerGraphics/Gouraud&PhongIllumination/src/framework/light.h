#pragma once

#include "framework.h"
#include "shader.h"

//This class contains all the info about the properties of the light
class Light
{
public:
	Vector3 position; //where is the light
	Vector3 diffuse_color; //the amount (and color) of diffuse
	Vector3 specular_color; //the amount (and color) of specular
	Vector3 ambient;

	Light();

	//possible method: uploads properties to shader uniforms
	void uploadToShader(Shader* shader);
	void uploadToShader2(Shader* shader);

	void setLight2_on();
	void setLight2_off();
	void setRed();
	void setGreen();
	void setBlue();
	void reset();
};







