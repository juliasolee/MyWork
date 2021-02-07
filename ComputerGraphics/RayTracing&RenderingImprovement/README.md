# Ray Tracing And Rendering Improvement

## Main Concept

Starting from a previous version of this project, which was a **Ray Tracer** that could render different materials as Phong, Diffuse, Glossy, Mirror and Glass (these last two using specular reflections and transmission); different shapes as spheres, infinite planes, and triangles (by ray intersection concepts) and also took into account the lightning conditions of the scene and was able to apply both direct illumination and global illumination.

<p align="center">
<img src="https://github.com/juliasolee/MyWork/blob/master/ComputerGraphics/RayTracing%26RenderingImprovement/Images/Direct.png" alt="Direct illumination" width="350px"> <img src="https://github.com/juliasolee/MyWork/blob/master/ComputerGraphics/RayTracing%26RenderingImprovement/Images/Global.png" alt="Global illumintaion" width="350px">
</p>

From there, we implemented a few improvements as we wanted to focus on meshes and deepen into how they are loaded and how they work:

## Loading a mesh - .obj file

The vertices of the polygons and the normal to each polygon of the 3D model stored are stored in a .obj file to encode the surface geometry of the model. 
We created a function called loadOBJ, that will read the file and save the values that interest us.

## Intersecting the mesh

As we stored the mesh, it is formed by a vector of triangles. To know if the rays intersect with this mesh we will check for all the triangles of the mesh and get the values of the intersection for the ones the ray does intersect with.

### Complex meshes

This method was alright for basic meshes as a cube, but for more complex meshes as, for example, a sphere, the result was not as we desired it.

<p align="center">
<img src="https://github.com/juliasolee/MyWork/blob/master/ComputerGraphics/RayTracing%26RenderingImprovement/Images/Complex.png" alt="Complex mesh result" width="500px">
</p>

### Acceleration method - Bounding Sphere Method

One the problems with a more complex mesh was that, as it has more triangles, the computation time goes up. To solve this we implemented the Bounding Sphere Acceleration method, which will create a sphere containing the mesh, adjusted as much as possible. This will make it possible to not waste computation time by checking intersections with points we know for sure are not part of the mesh.

### Rendering improvement - Barycentric Coordinates

The other problem was that the visual result was not smooth enough, as we were computing the intersection with the same normal for all the points of each triangle, assuming it was the same for all of them.
We fixed this by using barycentric coordinates: ponderate the normal of each point of the triangle with respect to the vertices' normal with its barycentric coefficient.

By doing this we went from flat shading to smooth shading.

<p align="center">
<img src="https://github.com/juliasolee/MyWork/blob/master/ComputerGraphics/RayTracing%26RenderingImprovement/Images/Final.png" alt="Final results" width="500px">
</p>

## Engine

This project was developed in C++ and OpenGL. It was done for the UPF course Synthetic Image.

## Possible extension

Two possible extensions of this project could be applying textures to the mesh or lowing the computation time by using the grid acceleration method.

## Acknowledgements and Contributions

**Developers:** Júlia Solé Cubilo, Irene Ubieto Alamillo

**Framework Development and Assistance:** Ricardo Marqués


