#ifndef SPHERE_H
#define SPHERE_H

#include "hittable_object.h"
#include "vec3.h"

class sphere : public hittable_object
{
public:
    sphere(const point3 &center, double radius, Material material) : center(center), radius(std::fmax(0, radius)), material(material) {}

    bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &rec) const override
    {
        // h = b/-2 = d dot (C - Q)
        vec3 oc = center - r.origin();
        double a = r.direction().length_squared();
        double h = dot(r.direction(), oc);
        double c = oc.length_squared() - radius * radius;

        double discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        double sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range. straight out the guide
        auto root = (h - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root)
        {
            root = (h + sqrtd) / a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.material = material;

        return true;
    }

private:
    point3 center;
    double radius;
    Material material;
};

#endif