#ifndef POLYGON_H
#define POLYGON_H

#include "hittable_object.h"
#include "vec3.h"

class polygon : public hittable_object
{
public:
    point3 vert_a, vert_b, vert_c;
    Material material;
    polygon(point3 a, point3 b, point3 c, Material material) : vert_a(a), vert_b(b), vert_c(c), material(material) {}

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