#include "directshader.h"
#include "../core/utils.h"
#include "../lightsources/pointlightsource.h"
#include <cmath>
#include "../shapes/mesh.h"

DirectShader::DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_)
{
	this->color = color_;
	this->maxDist = maxDist_;
	this->bgColor = bgColor_;
}

Vector3D DirectShader::computeColor(const Ray &r, const std::vector<Shape *> &objList, const std::vector<PointLightSource> &lsList) const
{
	
	Intersection its;
	int Ln = lsList.size();
	Vector3D color;
	double V;

		if (Utils::getClosestIntersection(r, objList, its)) {

			//Now I have the intersection filled with the details of the closest intersection: position p, normal and shape
	
			//First I obtain p, which is the intersection point between the point and the object
			Vector3D p = its.itsPoint;
			Vector3D wo = (r.o - p).normalized();
			Vector3D wr;


			if(its.shape->getMaterial().hasDiffuseOrGlossy()){ //Phong
				
				for (int s = 0; s < Ln; s++) {

					PointLightSource light = lsList.at(s); //Save each light source
					Vector3D wi = (light.getPosition() - p).normalized(); //Direction of the ray
					Vector3D Li = light.getIntensity(p);

					if (dot(wi, its.normal) > 0) {

						//We define a ray that goes from p (intersection) to Ls
						Ray *r_aux = new Ray();
						r_aux->o = p;
						r_aux->d = wi;
						r_aux->depth = 0;
						r_aux->minT = 0.0001;
						r_aux->maxT = (lsList.at(s).getPosition() - p).length();

						//In case there is an intersection, there is an occlusion and we pass to another light source

						//In case there isn't an intersection:
						if (!Utils::hasIntersection(*r_aux, objList))
						{
							//We compute the reflectance wr
							wr = its.shape->getMaterial().getReflectance(its.normal, wo, wi);
							//The color is a summatory
							color = color + (Utils::multiplyPerCanal(Li, wr));
						}
					}
				}

			}
			else if (its.shape->getMaterial().hasSpecular()) { //Mirror	

				//Compute the reflection direction
				wr = its.shape->getMaterial().getReflectance(its.normal, wo, wo);
				Ray reflectionRay(p, wr, r.depth + 1);
				//Compute the color with recursivity
				color = computeColor(reflectionRay, objList, lsList);

			}
			else if (its.shape->getMaterial().hasTransmission()) { //Transmissive
				
				Vector3D aux_n = its.normal;
				float eta_t = its.shape->getMaterial().getIndexOfRefraction();
				
				if (dot(its.normal, wo) < 0) { //If we are exiting the material , we invert the normal and the quocient
					aux_n = - aux_n; 
					eta_t = 1.0 / eta_t;
				}
				
				double alpha_cos = dot(wo, aux_n);

				if (!Utils::isTotalInternalReflection(eta_t, alpha_cos, alpha_cos)) {
					
					//If is not total internal reflection the beta cosine can be computed
					double beta_cos = sqrt(1.0 + (pow(eta_t, 2.0) * (pow(alpha_cos, 2.0) - 1.0)));
					
					//Compute transmission direction
					//wr = its.shape->getMaterial().getReflectance(aux_n, wo, wo);
					wr = -wo * eta_t + aux_n * (eta_t * alpha_cos - beta_cos);

					Ray reflectionRay(p, wr, r.depth + 1);
					reflectionRay.minT = 0.0001;
					color = computeColor(reflectionRay, objList, lsList);
					
					//color = (its.normal + Vector3D(1.0)) / 2.0;
					//color = Vector3D(1,0,0);
				}
				else { // In case there is total internal reflection, there is specular relfection (mirror)
					
					//Compute the reflection direction
					wr = (aux_n * (2 * dot(aux_n, wo)) - wo).normalized();

					Ray reflectionRay(p, wr, r.depth + 1);
					reflectionRay.minT = 0.0001;
					color = computeColor(reflectionRay, objList, lsList);

				}

			}
			return color;
		}
		return bgColor;	
}