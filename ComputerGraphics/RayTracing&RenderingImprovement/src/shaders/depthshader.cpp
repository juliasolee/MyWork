#include "depthshader.h"
#include "../core/utils.h"
#include <cmath>

DepthShader::DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_)
{
	this->color = color_;
	this->maxDist = maxDist_;
	this->bgColor = bgColor_;
}

Vector3D DepthShader::computeColor(const Ray &r, const std::vector<Shape *> &objList, const std::vector<PointLightSource> &lsList) const
{
	Intersection its;

	if (Utils::getClosestIntersection(r, objList, its))
	{
		//Computing the distance between two points
		Vector3D distAux = its.itsPoint - r.o;
		//double dist = sqrt(distAux.x + distAux.y + distAux.z);
		double dist = distAux.length();

		if (dist >= maxDist) {
			return bgColor;
		}
		else {
			return color * (1.0 - dist / maxDist);
		}
	}
	else
	{
		return bgColor;
	}
}