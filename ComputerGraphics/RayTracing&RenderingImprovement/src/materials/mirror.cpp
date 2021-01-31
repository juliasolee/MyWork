#include "mirror.h"
#include <iostream>

Mirror::Mirror(const Vector3D ks_)
{
	this->ks = ks_;

}

bool Mirror::hasSpecular() const
{
	// This material have specular component
	return true;
}
bool Mirror::hasTransmission() const
{
	return false;

}
bool Mirror::hasDiffuseOrGlossy() const
{
	return false;
}

Vector3D Mirror::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const
{

	Vector3D wr = n * (2 * dot(n, wo)) - wo;

	return wr;
}

double Mirror::getIndexOfRefraction() const
{
	std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
		<< std::endl;

	return -1;
}
