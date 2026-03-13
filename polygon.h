#ifndef POLYGON_H
#define POLYGON_H

#include "hittable_object.h"
#include "vec3.h"

class polygon : public hittable_object
{
public:
    std::vector<point3> vertices;
    vec3 normal;
    Material material;
    double distance;

    polygon(std::vector<point3> vertices, Material material) : vertices(vertices), material(material)
    {
        vec3 e1 = vertices[1] - vertices[0];
        vec3 e2 = vertices[2] - vertices[0];
        normal = unit_vector(cross(e1, e2));
        distance = -dot(vertices[0], normal);
    }
    bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &rec) const override
    {
        double denom = dot(normal, r.direction());
        // parrallel
        if (std::fabs(denom) == 0)
        {
            return false;
        }
        // hits plane
        double t = -(dot(r.origin(), normal) + distance) / denom;
        if (t < ray_tmin || t > ray_tmax)
        {
            return false;
        }

        point3 p = r.at(t);
        for (size_t i = 0; i < vertices.size(); i++)
        {
            point3 v0 = vertices[i];
            point3 v1 = vertices[(i + 1) % vertices.size()];
            vec3 edge = v1 - v0;
            vec3 vp = p - v0;
            if (dot(normal, cross(edge, vp)) < 0)
            {
                return false;
            }
        }
        rec.t = t;
        rec.p = p;
        rec.set_face_normal(r, normal);
        rec.material = material;

        return true;
    }
};

#endif