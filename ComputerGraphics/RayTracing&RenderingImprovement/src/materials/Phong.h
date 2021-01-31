#ifndef PHONG_H
#define PHONG_H

#include "material.h"

class Phong : public Material
{
public:

	Phong() = delete;
	Phong(const Vector3D kd_, const Vector3D ks_, int n_coeff_);

	virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const;
	virtual bool hasSpecular() const;
	virtual bool hasTransmission() const;
	virtual bool hasDiffuseOrGlossy() const;

	virtual double getIndexOfRefraction() const;



private:

	Vector3D kd;
	Vector3D ks;
	int n_coeff;
};

#endif // PHONG_H
