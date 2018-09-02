// Pathtracer 'Monsoon' by Anand Hotwani 2018.
//
//	Created for educational purposes. Based on structures of PBRT, Scratchapixel & Ray-Tracing In One Weekend
//
// To run:
//	
//	>> mkdir build
//	>> cd build
//	>> cmake ..
//	>> make
//
//	>> ./monsoon [samples]
//

#include <stdio.h>
#include <stdlib.h>
#include "time.h"

#include "geometry.h"
#include "material.h"
#include "shape.h"
#include "camera.h"
#include "scene.h"
#include "renderer.h"


template <> const Matrix44f Matrix44f::kIdentity = Matrix44f();

int main(int argc, char *argv[]) {

	time_t start, stop;
	time(&start);               // Start execution timer
	int samples = 32;            // Default samples per pixel

	if (argc == 2) samples = atoi(argv[1]);

	// Create Scene
	Scene scene = Scene();

	// Create RenderCam
	//Matrix44f renderCam = Matrix44f(49.651886, 0, -22.106444, 0, -4.822403, 53.041796, -10.831295, 0, 21.574042, 11.856328, 48.456092, 0, 38.029009, 35.678453, 107.141704, 1);	// TIGER
	Matrix44f renderCam = Matrix44f(53.315654, 0, 10.556784, 0, 0, 54.350756, 0, 0, -10.556784, 0, 53.315654, 0, -71.647173, 56.688284, 411.533242, 1);						// DOG
	//Matrix44f renderCam = Matrix44f(52.204954, 0, 10.716138, 0, 1.231682, 52.940272, -6.000289, 0, -10.64512, 6.125398, 51.85898, 0, 259.648336, 137.268575, -92.114582, 1);    // KITCHEN
	double angle = 33.; // DEFAULT IS 33
	int width = 960;
	int height = 540;
	Camera camera = Camera(renderCam, angle, 0, 0, width, height);



	// Create scene objects
	Matrix44f floor = Matrix44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -1003, 0, 1);
	//scene.Add(dynamic_cast<Shape*>(new Sphere(floor, 1000, Material(DIFFUSE, Vec3f(.025, .144, .138)))));

	Matrix44f wall = Matrix44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -1050, 1);
	//scene.Add(dynamic_cast<Shape*>(new Sphere(wall, 1000, Material(DIFFUSE, Vec3f(.617, .328, .205)))));

	Matrix44f rightWall = Matrix44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1010, 0, 0, 1);
	//scene.Add(dynamic_cast<Shape*>(new Sphere(rightWall, 1000, Material(DIFFUSE, Vec3f(.617, .328, .205)))));

	scene.Add(dynamic_cast<Shape*>(new Sphere(Matrix44f::kIdentity, 1, Material(DIFFUSE, Vec3f(0.3, 0, 0)))));

	Matrix44f xformHippo = Matrix44f(-1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 1.313816, 0.241444, 0, 1);
	//scene.Add(dynamic_cast<Shape*>(new TriangleMesh(xformHippo, "hippo.obj", Material(DIFFUSE, Vec3f(0.18)))));

	Matrix44f middleSphere = Matrix44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1.661024, -0.321387, -5, 1);
	//scene.Add(dynamic_cast<Shape*>(new Sphere(middleSphere, 1, Material(SPECULAR))));

	Matrix44f backSphere = Matrix44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 4.217509, -0.638817, -10, 1);
	//scene.Add(dynamic_cast<Shape*>(new Sphere(backSphere, 1, Material(DIFFUSE, Vec3f(0.09, 0.11, 0.09)))));

	// Create light
	Matrix44f light = Matrix44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -10.649108, 30.191502, -6.810769, 1);
	//scene.Add(dynamic_cast<Shape*>(new Sphere(light, 10, Material(EMISSIVE, Vec3f(0.8, 0.2, 0.6), Vec3f(15.2, 15.2, 15.2)))));


	
	
	/**
	 **	Tiger scene
	 **
	 **
	 **
	**/
#if 0

	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/tiger/background.obj", Material(DIFFUSE, Vec3f(0.194, 0.310, 0.378)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/tiger/tiger_light_B.obj", Material(EMISSIVE, Vec3f(1.0, 0.917, 0.917), Vec3f(3)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/tiger/tiger_light_A.obj", Material(EMISSIVE, Vec3f(1.0, 1.0, 1.0), Vec3f(3)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/tiger/tiger.obj", Material(DIFFUSE, Vec3f(0.947, 0.783, 0.513)))));
	//scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/tiger.obj", Material(SPECULAR))));
#endif	






	/**
	 **	Dog scene
	 **
	 **
	 **
	**/
	
#if 1
	
	// Geometry
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/dog/background.obj", Material(DIFFUSE, Vec3f(0.442, 0.340, 0.413)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/dog/dog.obj", Material(DIFFUSE, Vec3f(0.213, 0.442, 0.409)))));
	//scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "dog.obj", Material(EMISSIVE, Vec3f(0.213, 0.442, 0.409), Vec3f(15, 15, 15)))));

	// Calibration balls
	Matrix44f xformGreyball = Matrix44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 75.040614, 13.271885, 38.673819, 1);
	scene.Add(dynamic_cast<Shape*>(new Sphere(xformGreyball, 13, Material(DIFFUSE, Vec3f(0.18)))));

	Matrix44f xformChromeball = Matrix44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 106.775137, 13.271885, 38.673819, 1);
	scene.Add(dynamic_cast<Shape*>(new Sphere(xformChromeball, 13, Material(SPECULAR))));

	// Lights
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/dog/topLight.obj", Material(EMISSIVE, Vec3f(1.0, 0.96, 0.885), Vec3f(6)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/dog/sideLight.obj", Material(EMISSIVE, Vec3f(1.0, 0.854, 0.577), Vec3f(8)))));

#endif	



	/**
	 **	Kitchen Scene
	 **
	 **
	 **
	**/
	
#if 0

	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/background.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/bottle.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/clock.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/cup.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/cup02.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/knife.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/pan.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/spatula.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/stove.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/table.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/wall01.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/wall02.obj", Material(DIFFUSE, Vec3f(0.5)))));
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/wall03.obj", Material(DIFFUSE, Vec3f(0.5)))));
	
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/roomLight.obj", Material(EMISSIVE, Vec3f(.34, 0.71, 1.0), Vec3f(20)))));	
	scene.Add(dynamic_cast<Shape*>(new TriangleMesh(Matrix44f::kIdentity, "../obj/kitchen/sunLight.obj", Material(EMISSIVE, Vec3f(1.0, 0.7, 0.34), Vec3f(40)))));	
#endif	
	
	
	

	Renderer renderer = Renderer(&scene, &camera);  // Create renderer with our scene and camera
	renderer.Render(samples);                       // Render image to pixel buffer
	renderer.WriteImage("out.png");              // Save image

												 // Print duration information
	time(&stop);
	double diff = difftime(stop, start);
	int hrs = (int)diff / 3600;
	int mins = ((int)diff / 60) - (hrs * 60);
	int secs = (int)diff - (hrs * 3600) - (mins * 60);
	printf("\r>>> Rendering scene with %i spp: Completed in:\n %i hrs, %i mins, %i secs\n\n", samples, hrs, mins, secs);
	return 0;
}