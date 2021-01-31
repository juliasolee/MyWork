#include "utils.h"

Utils::Utils()
{ }

double Utils::degreesToRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}

Vector3D Utils::multiplyPerCanal(const Vector3D &v1, const Vector3D &v2)
{
    return Vector3D(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

bool Utils::hasIntersection(const Ray &ray, const std::vector<Shape*> &objectsList)
{
	
    // For each object on the scene...
    for(size_t objIndex = 0; objIndex < objectsList.size(); objIndex ++)
    {
          // Get the current object
          const Shape *obj = objectsList.at(objIndex);
		  if (obj->rayIntersectP(ray) == true) {
			  return true;
		  }
    }
 
    return false;
}

bool Utils::getClosestIntersection(const Ray &cameraRay, const std::vector<Shape*> &objectsList, Intersection &its)
{
    //std::cout << "Need to implement the function Utils::getClosestIntersection() in the file utils.cpp" << std::endl;
	bool aux = false;
	//Intersection itsAux;
	Vector3D distAux;
	double dist;

	for (size_t objIndex = 0; objIndex < objectsList.size(); objIndex++)
	{
		// Get the current object
		const Shape *obj = objectsList.at(objIndex);
		if (obj->rayIntersect(cameraRay, its) == true) {
			
			/*if (objIndex == 0) {
				itsAux = its;
			}
			else {
				if (its.itsPoint.z < itsAux.itsPoint.z) {
					itsAux = its;
				}
			}*/

			aux = true;
		}
	}

	//its = itsAux;
    return aux;
}

double interpolate(double val, double y0, double x0, double y1, double x1 )
{
    return (val-x0)*(y1-y0)/(x1-x0) + y0;
}

double getRed(double value)
{
    if (value > 0.5)
        return interpolate( value, 0.0, 0.5, 1.0, 1.0 );
    else
        return 0.0;
}

double getGreen(double value)
{
    if (value < 0.25)
        return 0.0;
    else if (value < 0.5)
        return interpolate(value, 0.0, 0.25, 1, 0.5);
    else if (value < 0.75)
        return interpolate(value, 1, 0.5, 0, 0.75);
    else
        return 0;
}

double getBlue(double value)
{
    if (value < 0.5)
        return interpolate(value, 1.0, 0.0, 0.0, 0.5);
    else
        return 0;
}


Vector3D Utils::scalarToRGB(double scalar)
{
    return Vector3D( getRed(scalar),
                getGreen(scalar),
                getBlue(scalar) );
}

Vector3D Utils::computeReflectionDirection(const Vector3D &rayDirection, const Vector3D &normal)
{
    // Compute the perfect reflection direction
    return normal * 2 * dot(normal, -rayDirection) + rayDirection;
}

bool Utils::isTotalInternalReflection(const double &eta, const double &cosThetaI,
                                      double &cosThetaT_out)
{
	double radicand = 1 + pow(eta, 2) * (pow(cosThetaI, 2) - 1);
	if (radicand < 0) {
		return true;
	}
    return false;
}

Vector3D Utils::computeTransmissionDirection(const Ray &r, const Vector3D &normal,
                                             const double &eta, const double &cosThetaI,
                                             const double &cosThetaT)
{
    return Vector3D(0);
}

double Utils::computeReflectanceCoefficient(const double &eta, const double &cosThetaI,
                                            const double &cosThetaT)
{
    return 0.0;
}

std::vector<std::string> Utils::tokenize(const std::string& source, const char* delimiters, bool process_strings)
{
	std::vector<std::string> tokens;

	std::string str;
	size_t del_size = strlen(delimiters);
	const char* pos = source.c_str();
	char in_string = 0;
	unsigned int i = 0;
	while (*pos != 0)
	{
		bool split = false;

		if (!process_strings || (process_strings && in_string == 0))
		{
			for (i = 0; i < del_size && *pos != delimiters[i]; i++);
			if (i != del_size) split = true;
		}

		if (process_strings && (*pos == '\"' || *pos == '\''))
		{
			if (!str.empty() && in_string == 0) //some chars remaining
			{
				tokens.push_back(str);
				str.clear();
			}

			in_string = (in_string != 0 ? 0 : *pos);
			if (in_string == 0)
			{
				str += *pos;
				split = true;
			}
		}

		if (split)
		{
			if (!str.empty())
			{
				tokens.push_back(str);
				str.clear();
			}
		}
		else
			str += *pos;
		pos++;
	}
	if (!str.empty())
		tokens.push_back(str);
	return tokens;
}



