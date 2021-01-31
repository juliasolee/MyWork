#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <string>
#include <vector>

#include "../shapes/shape.h"
#include "../shapes/sphere.h"
#include "../core/eqsolver.h"

//BoundingBox class
class BoundingBox
{
public:
	Vector3D center;
	Vector3D halfsize;
	BoundingBox() {}
	BoundingBox(Vector3D center, Vector3D halfsize) { this->center = center; this->halfsize = halfsize; };
};

struct SimpleTriangle
{ //a triangle with its three vertices

	Vector3D A;
	Vector3D B;
	Vector3D C;

	Vector3D nA;
	Vector3D nB;
	Vector3D nC;

	Shape *mesh;

	Vector3D getNormalWorld(Vector3D n);
	double SimpleTriangle::area(Vector3D a, Vector3D b, Vector3D c) const;
	Vector3D SimpleTriangle::barNormal(Vector3D q) const;
	bool rayIntersect(const Ray &ray, Intersection &its);
	bool rayIntersectP(const Ray &ray);

};

class Mesh : public Shape
{
public:
	Mesh() = delete;
	Mesh(const Matrix4x4 &t, Material *material_);

	Material *material;

	std::vector<Vector3D> vertices;
	std::vector<Vector3D> normals;
	std::vector<int> indices;

	//values to compute the boundingbox
	Vector3D min_v;
	Vector3D max_v;
	BoundingBox bound_box;
	Sphere *bound_sphere;

	float radius;

	std::vector<SimpleTriangle> triangles;

	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;

	bool loadOBJ(const char* filename);
};
#endif // MESH_H
#pragma once
