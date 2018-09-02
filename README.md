# Diya Renderer
A personal uni-directional path-tracer project. "Diya" is a small Indian oil lamp. 

This path-tracer is based on examples from Scratchapixel, Ray-Tracing in One Weekend and PBRT and is still work-in-progress.

# Features

* Monte Carlo sampling for brute force global illumination
* Triangle Mesh (.obj) rendering
* KD-Tree acceleration structure
* Diffuse, Specular and Emissive BRDF shaders
* Area lighting
* Simple multi-threaded with OpenMP
* Object-to-World and Cam-to-World matrices to directly read in Maya scenes.
* ACES RRT+ODT Display Transform for sRGB 

# Renders

![01](https://user-images.githubusercontent.com/10408010/44960199-1eb0f680-aef3-11e8-82f0-fe84fbbd008d.png)
![02](https://user-images.githubusercontent.com/10408010/44960210-3f794c00-aef3-11e8-9bd7-a84b44e31a39.png)
![kitchen](https://user-images.githubusercontent.com/10408010/44960214-4607c380-aef3-11e8-9961-70af32cc7a98.png)

# Run

```
>>> mkdir build
>>> cd build
>>> cmake ..
>>> make
```

And then
```
>>> ./diya [SAMPLES]
```
