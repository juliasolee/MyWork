#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include <string>

#include "shape.h"
#include "../core/eqsolver.h"

class Triangle : public Shape
{
public:
	Triangle() = delete;
	Triangle(Vector3D pA_, Vector3D pB_, Vector3D pC_, const Matrix4x4 &t, Material *material_);

	Vector3D getNormalWorld() const;

	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;

private:
	// The triangle is defined by 3 points
	Vector3D pA;
	Vector3D pB;
	Vector3D pC;
};


#endif // TRIANGLE_H
#pragma once