#include "transmissive.h"

Transmissive::Transmissive(float n2, const Vector3D trans)
{
	this->n2 = n2;
}

bool Transmissive::hasSpecular() const
{
	// This material have specular component
	return false;
}
bool Transmissive::hasTransmission() const
{
	return true;
}
bool Transmissive::hasDiffuseOrGlossy() const
{
	return false;
}

Vector3D Transmissive::getReflectance(const Vector3D &normal, const Vector3D &wo, const Vector3D &wi) const
{

	double alpha_cos = dot(wo, normal);
	float eta_t = n2;

	if (dot(normal, wo) < 0) { //If we are exiting the material , we invert the normal and the quocient
		eta_t = 1.0 / eta_t;
	}

	double beta_cos = sqrt(1.0 + (pow(eta_t, 2.0) * (pow(alpha_cos, 2.0) - 1.0)));

	Vector3D wt = -wo * eta_t + normal * (eta_t * alpha_cos - beta_cos);

	return wt;

}

double Transmissive::getIndexOfRefraction() const
{
	return n2;
}