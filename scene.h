#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "vec3.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"

struct CameraData
{
    point3 lookFrom;
    point3 lookAt;
    vec3 up;
    double fov;
};

struct Light
{
    vec3 direction;
    color intensity;
};

class scene
{
public:
    CameraData camera;
    Light light;
    hittable_list objects;
    color ambientLight;
    color backgroundColor;
};

#endif