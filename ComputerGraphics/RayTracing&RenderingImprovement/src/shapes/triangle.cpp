#include "triangle.h"
#include "infiniteplane.h"

Triangle::Triangle(Vector3D pA_, Vector3D pB_, Vector3D pC_, const Matrix4x4 &t_, Material *material_)
	: Shape(t_, material_), pA(pA_), pB(pB_), pC(pC_)
{}

// Return the normal in world coordinates
Vector3D Triangle::getNormalWorld() const
{
	//We compute the normal
	Vector3D vAB = pB - pA;
	Vector3D vAC = pC - pA;
	Vector3D n = cross(vAC, vAB);

	Matrix4x4 inverseTransposed;
	objectToWorld.transpose(inverseTransposed);
	Vector3D nWorld = inverseTransposed.transformVector(n);

	// Check whether applying the transform to a normalized
	// normal allways yields a normalized normal
	return(nWorld.normalized());
}

bool Triangle::rayIntersect(const Ray &ray, Intersection &its) const
{

	Ray r = worldToObject.transformRay(ray);

	Vector3D vAB = pB - pA;
	Vector3D vAC = pC - pA;
	Vector3D vCA = pA - pC;
	Vector3D vBC = pC - pB;

	Vector3D n = cross(vAC, vAB);

	//First we compute the intersection of the ray with the
	//plane defined by the triangle

	// Compute the denominator of the tHit formula
	double denominator = dot(r.d, n);

	// If the denominator is very close to zero, then the ray and the
	// plane are almost parallel
	if (std::abs(denominator) < Epsilon) {
		return false;
	}

	//Transform point pA to world coordinates

	// Effectivelly compute the intersection point
	double tHit = dot(pA - r.o, n) / denominator;
	Vector3D pHit = r.o + r.d * tHit;

	//Then we check if the intersection point belongs to the triangle
	Vector3D vAp = pHit - pA;
	Vector3D vBp = pHit - pB;
	Vector3D vCp = pHit - pC;
	Vector3D dAB = cross(vAp, vAB);
	Vector3D dBC = cross(vBp, vBC);
	Vector3D dCA = cross(vCp, vCA);
	dAB = dAB.normalized();
	dBC = dBC.normalized();
	dCA = dCA.normalized();

	if (dot(dAB, n) < 0) {
		return false;
	}
	if (dot(dBC, n) < 0) {
		return false;
	}
	if (dot(dCA, n) < 0) {
		return false;
	}

	// If we arrive here it is because there is an intersection
	// with the tested ray segment!

	// Is tHit outside the bounds of the ray segment we want to test intersecion with?
	if (tHit < r.minT || tHit > r.maxT)
		return false;

	r.maxT = tHit;

	//its.itsPoint = pHit;
	its.itsPoint = objectToWorld.transformPoint(pHit);
	its.normal = getNormalWorld();
	its.shape = this;

	return true;
}

bool Triangle::rayIntersectP(const Ray &ray) const
{

	Ray r = worldToObject.transformRay(ray);

	Vector3D vAB = pB - pA;
	Vector3D vAC = pC - pA;
	Vector3D vCA = pA - pC;
	Vector3D vBC = pC - pB;

	Vector3D n = cross(vAC, vAB);

	//First we compute the intersection of the ray with the
	//plane defined by the triangle

	// Compute the denominator of the tHit formula
	double denominator = dot(r.d, n);

	// If the denominator is very close to zero, then the ray and the
	// plane are almost parallel
	if (std::abs(denominator) < Epsilon) {
		return false;
	}

	//Transform point pA to world coordinates

	// Effectivelly compute the intersection point
	double tHit = dot(pA - r.o, n) / denominator;
	Vector3D pHit = r.o + r.d * tHit;

	//Then we check if the intersection point belongs to the triangle
	Vector3D vAp = pHit - pA;
	Vector3D vBp = pHit - pB;
	Vector3D vCp = pHit - pC;
	Vector3D dAB = cross(vAp, vAB);
	Vector3D dBC = cross(vBp, vBC);
	Vector3D dCA = cross(vCp, vCA);
	dAB = dAB.normalized();
	dBC = dBC.normalized();
	dCA = dCA.normalized();

	if (dot(dAB, n) < 0) {
		return false;
	}
	if (dot(dBC, n) < 0) {
		return false;
	}
	if (dot(dCA, n) < 0) {
		return false;
	}

	// If we arrive here it is because there is an intersection
	// with the tested ray segment!

	// Is tHit outside the bounds of the ray segment we want to test intersecion with?
	if (tHit < r.minT || tHit > r.maxT)
		return false;

	r.maxT = tHit;

	return true;
}


