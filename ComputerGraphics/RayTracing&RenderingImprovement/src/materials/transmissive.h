
#ifndef TRANSMISSIVE_H
#define TRANSMISSIVE_H

#include "material.h"

class Transmissive : public Material
{
public:

	Transmissive() = delete;
	Transmissive(float n2, const Vector3D trans);

	virtual Vector3D getReflectance(const Vector3D &normal, const Vector3D &wo, const Vector3D &wi) const;

	virtual bool hasSpecular() const;
	virtual bool hasTransmission() const;
	virtual bool hasDiffuseOrGlossy() const;

	virtual double getIndexOfRefraction() const;


private:

	double n2;

};

#endif // TRANSMISSIVE_H
