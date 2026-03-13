#include <iostream>
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "hittable_object.h"
#include "hittable_list.h"
#include "sphere.h"
#include "polygon.h"
#include "scene.h"
#include "scene_reader.h"

color ray_color(const ray &r, const scene &scene)
{
    hit_record rec;

    if (scene.objects.hit(r, 0, INFINITY, rec))
    {
        Material material = rec.material;
        Light light;
        light.direction = scene.light.direction;
        light.intensity = scene.light.intensity;
        color ambient_light = scene.ambientLight;
        color od = material.od;
        color os = material.os;
        double ka = material.ka;
        double kd = material.kd;
        double ks = material.ks;
        double kgls = material.kgls;

        color background_color = scene.backgroundColor;
        // phong: I = I=ka​Ia​Od​+kd​Ip​Od​(N⋅L)+ks​Ip​Os​(V⋅R)kgls​
        vec3 N = unit_vector(rec.normal);
        vec3 L = unit_vector(light.direction);
        vec3 V = unit_vector(-r.direction());
        vec3 R = unit_vector((2 * N) * dot(N, L) - L);

        auto ambient = ka * ambient_light * od;
        auto diffuse = kd * light.intensity * od * std::max(0.0, dot(N, L));
        auto VR = std::max(0.0, dot(V, R));
        auto specular = ks * light.intensity * os * pow(VR, kgls);
        return ambient + diffuse + specular;
    }
    vec3 unit_direction = unit_vector(r.direction());
    return scene.backgroundColor;
}

int main(int argc, char *argv[])
{
    std::string filename = "scene1.txt";
    if (argc > 1)
    {
        filename = argv[1];
    }
    scene scene = read_scene(filename);

    // Image
    int image_width = 400;
    int image_height = 400;
    auto aspect_ratio = double(image_width) / image_height;

    // Camera
    point3 lookFrom = scene.camera.lookFrom;
    point3 lookAt = scene.camera.lookAt;
    vec3 up = scene.camera.up;
    double fov = scene.camera.fov;
    // u, v, w
    vec3 w = unit_vector(lookFrom - lookAt);
    vec3 u = unit_vector(cross(up, w));
    vec3 v = cross(w, u);

    auto focal_length = (lookFrom - lookAt).length();
    double theta = fov * M_PI / 180.0;
    double h = tan(theta / 2);
    double viewport_height = h * focal_length;
    double viewport_width = aspect_ratio * viewport_height;
    auto camera_center = lookFrom;

    // horizontal/vertical viewport vectors
    auto viewport_u = viewport_width * u;
    auto viewport_v = viewport_height * -v;

    // horizontal/vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // upper left pixel
    auto viewport_upper_left = lookFrom - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center; // not unit vector becuase guide suggests it's better that way.. like this whole project
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, scene);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.           \n";
}