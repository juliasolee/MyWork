#include <iostream>
#include <ctime>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"

#include "shapes/sphere.h"
#include "shapes/infiniteplane.h"
#include "shapes/triangle.h"
#include "shapes/mesh.h"

#include "lightsources/pointlightsource.h"

#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/intersectionshader.h"
#include "shaders/normalshader.h"
#include "shaders/depthshader.h"
#include "shaders/directshader.h"

#include "materials/phong.h"
#include "materials/mirror.h"
#include "materials/transmissive.h"

using namespace std;

void buildSceneCornellBox(Camera* &cam, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 1, -3));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);
	

	/* ********* */
	/* Materials */
	/* ********* */
	Material *blueDiffuse = new Phong(Vector3D(0.3, 0.2, 0.7), Vector3D(0, 0, 0), 100);

	Material *mirror = new Mirror(Vector3D(1, 0.9, 0.85));
	Material *transmissive = new Transmissive(1.1, Vector3D(1));
	Material *red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.7, 0.2), 100);

	/* ******* */
	/* Objects */
	/* ******* */
	objectsList = new std::vector<Shape*>;
	double offset = 3.0;
	Matrix4x4 idTransform;
	// Construct the Cornell Box
	Shape *leftPlan = new InfinitePlane(Vector3D(-offset, 0, 0), Vector3D(1, 0, 0), blueDiffuse);
	Shape *rightPlan = new InfinitePlane(Vector3D(offset, 0, 0), Vector3D(-1, 0, 0), blueDiffuse);
	Shape *topPlan = new InfinitePlane(Vector3D(0, offset, 0), Vector3D(0, -1, 0), blueDiffuse);
	Shape *bottomPlan = new InfinitePlane(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), blueDiffuse);
	Shape *backPlan = new InfinitePlane(Vector3D(0, 0, 20), Vector3D(0, 0, -1), blueDiffuse);
	//objectsList->push_back(leftPlan);
	//objectsList->push_back(rightPlan);
	//objectsList->push_back(topPlan);
	//objectsList->push_back(bottomPlan);
	//objectsList->push_back(backPlan);

	/* ****** */
	/* Mesh */
	/* ****** */
	Matrix4x4 meshTransform1;
	meshTransform1 = meshTransform1.translate(Vector3D(0, 0, 13));
	Mesh *m1 = new Mesh(meshTransform1, red_100);
	if (m1->loadOBJ("data/sphere.obj")) {
		objectsList->push_back(m1);
	}

	Matrix4x4 sphereTransform1;
	sphereTransform1 = sphereTransform1.translate(Vector3D(0.0, 0.5, 0));
	Shape *s1 = new Sphere(0.5, sphereTransform1, red_100);
	//objectsList->push_back(s1);


	/* ****** */
	/* Lights */
	/* ****** */
	lightSourceList = new std::vector<PointLightSource>;
	Vector3D lightPosition1 = Vector3D(0, 4, 0);
	Vector3D lightPosition2 = Vector3D(0, offset - 1, 0);
	Vector3D lightPosition3 = Vector3D(0, 7,3);
	Vector3D lightPosition4 = Vector3D(0, 0, -4);
	Vector3D intensity = Vector3D(20, 20, 20); // Radiant intensity (watts/sr)
	PointLightSource pointLS1(lightPosition1, intensity);
	PointLightSource pointLS2(lightPosition2, intensity);
	PointLightSource pointLS3(lightPosition3, intensity);
	PointLightSource pointLS4(lightPosition4, intensity);
	lightSourceList->push_back(pointLS1);
	lightSourceList->push_back(pointLS2);
	lightSourceList->push_back(pointLS3);
	lightSourceList->push_back(pointLS4);

}

void raytrace(Camera* &cam, Shader* &shader, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
	unsigned int sizeBar = 40;

	size_t resX = film->getWidth();
	size_t resY = film->getHeight();

	// Main raytracing loop
	// Out-most loop invariant: we have rendered lin lines
	for (size_t lin = 0; lin < resY; lin++)
	{
		// Show progression
		if (lin % (resY / sizeBar) == 0)
			std::cout << ".";

		// Inner loop invariant: we have rendered col columns
		for (size_t col = 0; col < resX; col++)
		{
			// Compute the pixel position in NDC
			double x = (double)(col + 0.5) / resX;
			double y = (double)(lin + 0.5) / resY;

			// Generate the camera ray
			Ray cameraRay = cam->generateRay(x, y);

			// Compute ray color according to the used shader
			Vector3D pixelColor = shader->computeColor(cameraRay, *objectsList, *lightSourceList);

			// Store the pixel color
			film->setPixelValue(col, lin, pixelColor);
		}
	}
}

int main()
{
	std::string separator = "\n----------------------------------------------\n";
	std::string separatorStar = "\n**********************************************\n";
	std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

	// Create an empty film
	Film *film;
	film = new Film(720, 576);

	// Declare the shader
	Vector3D bgColor(0, 0, 0); // Background color (for rays which do not intersect anything)

	//Shader *shader = new IntersectionShader(Vector3D(0.4, 1, 0.4), bgColor);
	//Shader *shader = new NormalShader(bgColor);
	//Shader *shader = new DepthShader(Vector3D(0.4, 1, 0.4), 7, bgColor);
	Shader *shader = new DirectShader(Vector3D(0.4, 1, 0.4), 8, bgColor);

	// Declare pointers to all the variables which describe the scene
	Camera *cam;
	std::vector<Shape*> *objectsList;
	std::vector<PointLightSource> *lightSourceList;

	buildSceneCornellBox(cam, film, objectsList, lightSourceList);

	time_t current_time;
	current_time = time(NULL);
	//std::cout << "\nSPHERE LOCATED AT (0,0,13) WITH BOUNDING SPHERE\n"<< std::endl;
	//std::cout << "\nBefore raytrace:" << current_time << "\n" << std::endl;
	
	// Launch some rays!
	raytrace(cam, shader, film, objectsList, lightSourceList);
	current_time = time(NULL);
	//std::cout << "\n\nAfter raytrace:" << current_time << "\n" << std::endl;
	
	// Save the final result to file
	std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
	film->save();

	std::cout << "\n\n" << std::endl;
	return 0;
}


