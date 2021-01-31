#include "phong.h"
#include <iostream>

Phong::Phong(const Vector3D kd_, const Vector3D ks_, int n_coeff_)
{
	this->kd = kd_;
	this->ks = ks_;
	this->n_coeff = n_coeff_;
}

bool Phong::hasSpecular() const
{
	// This material does not have specular component
	return false;
}
bool Phong::hasTransmission() const
{
	return false;
}
bool Phong::hasDiffuseOrGlossy() const
{
	return true;
}

Vector3D Phong::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const
{

	Vector3D wr = n * (2 * dot(n, wi)) - wi;
	Vector3D rd = kd * dot(wi, n);
	Vector3D rs = ks * pow(dot(wo, wr), n_coeff);
	Vector3D r = rd + rs;

	return r;
}

double Phong::getIndexOfRefraction() const
{
	std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
		<< std::endl;

	return -1;
}