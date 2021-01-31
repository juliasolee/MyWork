#include "mesh.h"
#include "../core/utils.h"
#include "sys/stat.h"

Mesh::Mesh(const Matrix4x4 &t_, Material *material_)
	: Shape(t_, material_), material(material_)

{}

bool Mesh::loadOBJ(const char* filename)
{
	struct stat stbuffer;

	FILE* f = fopen(filename, "rb");
	if (f == NULL)
	{
		std::cerr << "File not found: " << filename << std::endl;
		return false;
	}

	stat(filename, &stbuffer);

	unsigned int size = stbuffer.st_size;
	char* data = new char[size + 1];
	fread(data, size, 1, f);
	fclose(f);
	data[size] = 0;

	char* pos = data;
	char line[255];
	int i = 0;

	std::vector<Vector3D> indexed_positions;
	std::vector<Vector3D> indexed_normals;

	//Initializing the values of the minimum and maximum vertex as very large to later acotate their values 
	const float max_float = 10000000;
	const float min_float = -10000000;
	min_v.set(max_float, max_float, max_float);
	max_v.set(min_float, min_float, min_float);

	unsigned int vertex_i = 0;

	//parse file
	while (*pos != 0)
	{
		if (*pos == '\n') pos++;
		if (*pos == '\r') pos++;

		//read one line
		i = 0;
		while (i < 255 && pos[i] != '\n' && pos[i] != '\r' && pos[i] != 0) i++;
		memcpy(line, pos, i);
		line[i] = 0;
		pos = pos + i;

		if (*line == '#' || *line == 0) continue; //comment

		//tokenize line
		std::vector<std::string> tokens = Utils::tokenize(line, " ");

		if (tokens.empty()) continue;

		if (tokens[0] == "v" && tokens.size() == 4) //Read the vertex
		{
			Vector3D v((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), (float)atof(tokens[3].c_str()));
			indexed_positions.push_back(v);

			//*** To compute the bounding box we need aabb_min and aabb_max ***
			//Minimum point with respect the local coordinates of the mesh
			if (v.x < min_v.x) min_v.x = v.x;
			if (v.y < min_v.y) min_v.y = v.y;
			if (v.z < min_v.z) min_v.z = v.z;

			//Minimum point with respect the local coordinates of the mesh
			if (v.x > max_v.x) max_v.x = v.x;
			if (v.y > max_v.y) max_v.y = v.y;
			if (v.z > max_v.z) max_v.z = v.z;
			
		}
		else if (tokens[0] == "vn" && tokens.size() == 4) //Read the normals
		{
			Vector3D v((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), (float)atof(tokens[3].c_str()));
			indexed_normals.push_back(v);
		}
		else if (tokens[0] == "f" && tokens.size() >= 4) //Read the triangles (faces)
		{
			Vector3D v1, v2, v3;
			v1.parseFromText(tokens[1].c_str(), '/');

			for (unsigned int iPoly = 2; iPoly < tokens.size() - 1; iPoly++)
			{

				v2.parseFromText(tokens[iPoly].c_str(), '/');
				v3.parseFromText(tokens[iPoly + 1].c_str(), '/');

				SimpleTriangle t; //Create the triangle and push it to the vector of triangles

				//Save the vertex of the triangle
				t.A = indexed_positions[(unsigned int)v1.x - 1];
				t.B = indexed_positions[(unsigned int)v2.x - 1];
				t.C = indexed_positions[(unsigned int)v3.x - 1];

				//Save the indexed vertex
				vertices.push_back(indexed_positions[(unsigned int)(v1.x) - 1]);
				vertices.push_back(indexed_positions[(unsigned int)(v2.x) - 1]);
				vertices.push_back(indexed_positions[(unsigned int)(v3.x) - 1]);
				vertex_i += 3;

				if (indexed_normals.size() > 0)
				{
					//Save the normals of each vertex of the triangle
					t.nA = indexed_normals[(unsigned int)v1.z - 1];
					t.nB = indexed_normals[(unsigned int)v2.z - 1];
					t.nC = indexed_normals[(unsigned int)v3.z - 1];

					//Save the indexed normals
					normals.push_back(indexed_normals[(unsigned int)(v1.z) - 1]);
					normals.push_back(indexed_normals[(unsigned int)(v2.z) - 1]);
					normals.push_back(indexed_normals[(unsigned int)(v3.z) - 1]);
				}

				t.mesh = this; //Triangle has a pointer to its mesh
				triangles.push_back(t); //Save the triangle
			}
		}
	}

	//We create the bounding box
	bound_box.center = (min_v + max_v) * 0.5; //Half of the distance between the minimum and the maximum
	bound_box.halfsize = (max_v - bound_box.center);//Halfsize is the distance between the centre and one of the corners (max_v for example)
	
	//We create the bounding sphere that we are going to use mor easily than the bounding box
	radius = (float)fmax(max_v.length(), min_v.length());//Returns the larger value
	bound_sphere = new Sphere(radius, objectToWorld, material);
}

bool Mesh::rayIntersect(const Ray &ray, Intersection &its) const
{
	bool aux = false;//auxiliar variable to store if the ray intersects or not with the mesh
	Ray r = ray;//Auxiliar ray in order that the ray isn't modified in the rayIntersectionP function of the sphere
	Intersection itsAux;
	bool hasEntered = false;

	//First checking the intersection with a simple sphere
	if (bound_sphere->rayIntersectP(r) == true) {
		
		//If the ray has intersect the sphere, we go through all the triangles of the mesh to test the intersection
		for (int i = 0; i < triangles.size(); i++) {
			SimpleTriangle t = triangles.at(i);
			if (t.rayIntersect(ray, its) == true) {//Testing the intersection with each triangle of the mesh
				
				aux = true;//We will return true
			}
		}
	}

	return aux;
}

bool Mesh::rayIntersectP(const Ray &ray) const
{

	//The same as the rayIntersect function but now it only checks if it intersects or not (it doesn't store the intersection)
	
	bool aux = false;

	Ray r = ray;
	if (bound_sphere->rayIntersectP(r) == true) {

		for (int i = 0; i < triangles.size(); i++) {

			SimpleTriangle t = triangles.at(i);
			if (t.rayIntersectP(ray) == true) {//Testing the intersection with each triangle of the mesh
				aux = true;//We will return true
			}
		}
	}
	
	return aux;
}



// Return the normal in world coordinates
Vector3D SimpleTriangle::getNormalWorld(Vector3D n)
{
	Matrix4x4 inverseTransposed;
	mesh->objectToWorld.transpose(inverseTransposed);
	Vector3D nWorld = inverseTransposed.transformVector(n);

	// Check whether applying the transform to a normalized
	// normal allways yields a normalized normal
	return(nWorld.normalized());
}

Vector3D SimpleTriangle::barNormal(Vector3D q) const
{
	
	//Calculate the barycentric ponderations
	double alpha = (-(q.x - B.x)*(A.y - B.y) + (q.y - B.y)*(A.x - B.x)) / (-(C.x - B.x)*(A.y - B.y) + (C.y - B.y)*(A.x - B.x));
	double beta = (-(q.x - A.x)*(C.y - A.y) + (q.y - A.y)*(C.x - A.x)) / (-(B.x - A.x)*(C.y - A.y) + (B.y - A.y)*(C.x - A.x));
	double gamma = 1 - alpha - beta;
	
	//Calculate the pondered normal
	Vector3D n_bar;
	n_bar = ((nA * alpha) + (nB * beta) + (nC * gamma)) / ((nA * alpha) + (nB * beta) + (nC * gamma)).length();

	return n_bar.normalized();
}

bool SimpleTriangle::rayIntersect(const Ray &ray, Intersection &its)
{
	Ray r = mesh->worldToObject.transformRay(ray);

	Vector3D vAB = B - A;
	Vector3D vAC = C - A;
	Vector3D vCA = A - C;
	Vector3D vBC = C - B;

	Vector3D n = cross(vAC, vAB);
	
	if (dot(-r.d, n) < 0)
	{
		n = -n;
		//In this case, we must put the vertexts in clock-wise order
		
		Vector3D aux_A = A;
		A = C;
		C = aux_A;
		
		vAB = B - A;
		vAC = C - A;
		vCA = A - C;
		vBC = C - B;
	
	}
	
	//First we compute the intersection of the ray with the
	//plane defined by the triangle

	// Compute the denominator of the tHit formula
	double denominator = dot(r.d, n);

	// If the denominator is very close to zero, then the ray and the
	// plane are almost parallel
	if (std::abs(denominator) < Epsilon) {
		return false;
	}

	// Effectivelly compute the intersection point
	double tHit = dot(A - r.o, n) / denominator;//A because is a point that belongs to the triangle

	// Is tHit outside the bounds of the ray segment we want to test intersecion with?
	if (tHit < r.minT || tHit > r.maxT)
		return false;

	Vector3D pHit = r.o + r.d * tHit;


	//Then we check if the intersection point belongs to the triangle
	Vector3D vAp = pHit - A;
	Vector3D vBp = pHit - B;
	Vector3D vCp = pHit - C;
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

	ray.maxT = tHit;

	//its.itsPoint = pHit;
	its.itsPoint = mesh->objectToWorld.transformPoint(pHit);
	n = barNormal(its.itsPoint);//Normal in the tHit point with the barycentric coordinates
	its.normal = getNormalWorld(n);//We use the normal of the obj file to fill the intersection structure
	its.shape = mesh;

	return true;
}

bool SimpleTriangle::rayIntersectP(const Ray &ray)
{

	Ray r = mesh->worldToObject.transformRay(ray);

	Vector3D vAB = B - A;
	Vector3D vAC = C - A;
	Vector3D vCA = A - C;
	Vector3D vBC = C - B;

	Vector3D n = cross(vAC, vAB);

	//Vector3D n = normal;

	if (dot(-r.d, n) < 0)
	{
		n = -n;
		//In this case, we must put the vertexts in clock-wise order
		Vector3D aux_A = A;
		A = C;
		C = aux_A;

		vAB = B - A;
		vAC = C - A;
		vCA = A - C;
		vBC = C - B;
	}

	
	//First we compute the intersection of the ray with the
	//plane defined by the triangle

	// Compute the denominator of the tHit formula
	double denominator = dot(r.d, n);

	// If the denominator is very close to zero, then the ray and the
	// plane are almost parallel
	if (std::abs(denominator) < Epsilon) {
		return false;
	}

	// Effectivelly compute the intersection point
	double tHit = dot(A - r.o, n) / denominator;

	// Is tHit outside the bounds of the ray segment we want to test intersecion with?
	if (tHit < r.minT || tHit > r.maxT)
		return false;

	Vector3D pHit = r.o + r.d * tHit;

	//Then we check if the intersection point belongs to the triangle
	Vector3D vAp = pHit - A; 
	Vector3D vBp = pHit - B;
	Vector3D vCp = pHit - C;
	Vector3D dAB = cross(vAp, vAB); //New normals
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

	r.maxT = tHit;

	return true;
}