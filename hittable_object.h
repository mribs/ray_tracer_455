#ifndef HITTABLE_OBJECT_H
#define HITTABLE_OBJECT_H

#include "ray.h"
#include "color.h"

struct Material
{
    color od, os;
    double ka, kd, ks, kgls;
};

class hit_record
{
public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    Material material;

    void set_face_normal(const ray &r, const vec3 &outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable_object
{
public:
    virtual ~hittable_object() = default;

    virtual bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &rec) const = 0;
};

#endif