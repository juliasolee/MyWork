#include "light.h"
#include "shader.h"



Light::Light()
{

	//Difusa: llum que arriba directament de la font de llum i rebota en totes direccions. 
	//Especular: llum que arriba directament de la font de llum i rebota en una direccio, segons la normal de la superficie
	//Ambient: llum que arriba de les parets, mobles, etc. Es reflexa en totes direccions simultaniament

	position.set(0, 20, 5);
	diffuse_color.set(1.0, 1.0, 1.0);
	specular_color.set(1.0, 1.0, 1.0);
	ambient.set(0.5, 0.5, 0.5);

}

void Light::uploadToShader(Shader* shader) {

	//Pugem aquestes components al shader
	shader->setUniform3("ia", ambient);
	shader->setUniform3("id", diffuse_color);
	shader->setUniform3("is", specular_color);
	shader->setUniform3("pos_light", position);
}

void Light::uploadToShader2(Shader* shader) {

	//Pugem aquestes components al shader
	shader->setUniform3("ia2", ambient);
	shader->setUniform3("id2", diffuse_color);
	shader->setUniform3("pos_light2", position);
}

void Light::setLight2_on() {
	position.set(0, 20, 5);
	diffuse_color.set(1.0, 1.0, 1.0);
	specular_color.set(1.0, 1.0, 1.0);
}

void Light::setLight2_off() {
	position.set(0, 20, 5);
	diffuse_color.set(0.0, 0.0, 0.0);
	specular_color.set(0.0, 0.0, 0.0);
}

void Light::setRed() {
	diffuse_color.set(1.0, 0.0, 0.0);
}

void Light::setGreen() {
	diffuse_color.set(0.0, 1.0, 0.0);
}

void Light::setBlue() {
	diffuse_color.set(0.0, 0.0, 1.0);
}

void Light::reset() {
	diffuse_color.set(1.0, 1.0, 1.0);
	specular_color.set(1.0, 1.0, 1.0);
	ambient.set(0.5, 0.5, 0.5);
}





