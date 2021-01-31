#include "vector3d.h"

Vector3D::Vector3D() : x(0), y(0), z(0)
{
}

Vector3D::Vector3D(double a) : x(a), y(a), z(a)
{
}

Vector3D::Vector3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_)
{
}

Vector3D::Vector3D(const Vector3D &v_) : x(v_.x), y(v_.y), z(v_.z)
{
}

// Sum two vectors and return the result as a new object
Vector3D Vector3D::operator+(const Vector3D &v) const
{
    return Vector3D(x + v.x, y + v.y, z + v.z);
}

// Subtract two vectors and return the result as a new object
Vector3D Vector3D::operator-(const Vector3D &v) const
{
    return Vector3D(x - v.x, y - v.y, z - v.z);
}

// Return the negative of the vector as a new object
Vector3D Vector3D::operator-() const
{
    return Vector3D(-x, -y, -z);
}

// Multiply a vector by a scalar and return the result as a new object
Vector3D Vector3D::operator*(const double a) const
{
    return Vector3D(x*a, y*a, z*a);
}

// Divide a vector by a scalar and return the result as a new object
Vector3D Vector3D::operator/(const double a) const
{
    return Vector3D(x/a, y/a, z/a);
}

// Add a vector to the current one
Vector3D& Vector3D::operator+=(const Vector3D &v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

// Subtract a vector to the current one
Vector3D& Vector3D::operator-=(const Vector3D &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}

// Multiply the current vector by a scalar
Vector3D& Vector3D::operator*=(const double a)
{
    x *= a;
    y *= a;
    z *= a;

    return *this;
}

// Divide the current vector by a scalar
Vector3D& Vector3D::operator/=(const double a)
{
    x /= a;
    y /= a;
    z /= a;

    return *this;
}

// Squared length of the current vector
double Vector3D::lengthSq() const
{
    return x*x + y*y + z*z;
}

// Length of the current vector
double Vector3D::length() const
{
    //[T] : check line 572 of vector.h in mitsuba
    return std::sqrt(lengthSq());
}

//Distance between two vectors
double Vector3D::dist(Vector3D q) const
{
	return std::sqrt(pow((q.x - x), 2) + pow((q.y - y), 2) + pow((q.z - z), 2));
}

// Returns the normalized version of the current vector
Vector3D Vector3D::normalized() const
{
    return (*this)/length();
}

void Vector3D::parseFromText(const char* text, const char separator)
{
	int pos = 0;
	char num[255];
	const char* start = text;
	const char* current = text;

	while (1)
	{
		if (*current == separator || (*current == '\0' && current != text))
		{
			strncpy(num, start, current - start);
			num[current - start] = '\0';
			start = current + 1;
			if (num[0] != 'x') //¿?
				switch (pos)
				{
				case 0: x = (float)atof(num); break;
				case 1: y = (float)atof(num); break;
				case 2: z = (float)atof(num); break;
				default: return; break;
				}

			++pos;
			if (*current == '\0')
				break;
		}

		++current;
	}
};

// Stream insertion operator
std::ostream& operator<<(std::ostream& out, const Vector3D &v)
{
    out << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return out;
}

