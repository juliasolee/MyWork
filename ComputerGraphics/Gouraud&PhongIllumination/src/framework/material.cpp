#include "material.h"
#include "shader.h"



Material::Material()
{
	//Difusa: llum que arriba directament de la font de llum i rebota en totes direccions. 
	//Especular: llum que arriba directament de la font de llum i rebota en una direccio, segons la normal de la superficie
	//Ambient: llum que arriba de les parets, mobles, etc. Es reflexa en totes direccions simultaniament

	ambient.set(1, 1, 1); //reflected ambient light
	diffuse.set(1, 1, 1); //reflected diffuse light
	specular.set(1, 1, 1); //reflected specular light
	shininess = 50.0; //glosiness coefficient (plasticity)
}

void Material::uploadToShader(Shader* shader) {

	//Pugem aquestes components al shader
	shader->setUniform3("kd", diffuse);
	shader->setUniform3("ks", specular);
	shader->setUniform3("ka", ambient);
	shader->setUniform1("alpha", shininess);
}

void Material::setMaterial(Vector3 ambient, Vector3 diffuse, Vector3 specular, float shininess) {

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}