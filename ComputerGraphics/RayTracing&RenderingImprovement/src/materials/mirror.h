
#ifndef MIRROR_H
#define MIRROR_H

#include "material.h"

class Mirror : public Material
{
public:

	Mirror() = delete;
	Mirror(const Vector3D ks_);

	virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const;
	
	virtual bool hasSpecular() const;
	virtual bool hasTransmission() const;
	virtual bool hasDiffuseOrGlossy() const;

	virtual double getIndexOfRefraction() const;


private:

	Vector3D ks;

};

#endif // MIRROR_H
