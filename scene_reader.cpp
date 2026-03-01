#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "scene.h"

scene read_scene(const std::string &filename)
{

    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Could not open scene file\n";
        exit(1);
    }

    scene scene;
    std::string line, keyword;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        std::stringstream stream(line);
        stream >> keyword;
        /* CameraLookAt 0 0 0
        CameraLookFrom 0 0 1
        CameraLookUp 0 1 0
        FieldOfView 90

        DirectionToLight 0.0 1.0 0.0
        LightColor 1.0 1.0 1.0
        AmbientLight 0.0 0.0 0.0
        BackgroundColor 0.2 0.2 0.2

        # purple sphere
        Sphere
        */
        if (keyword == "CameraLookAt")
        {
            stream >> scene.camera.lookAt;
        }
        else if (keyword == "CameraLookFrom")
        {
            stream >> scene.camera.lookFrom;
        }
        else if (keyword == "CameraLookUp")
        {
            stream >> scene.camera.up;
        }
        else if (keyword == "FieldOfView")
        {
            stream >> scene.camera.fov;
        }
        else if (keyword == "DirectionToLight")
        {
            stream >> scene.light.direction;
        }
        else if (keyword == "LightColor")
        {
            stream >> scene.light.intensity;
        }
        else if (keyword == "AmbientLight")
        {
            stream >> scene.ambientLight;
        }
        else if (keyword == "BackgroundColor")
        {
            stream >> scene.backgroundColor;
        }
        else if (keyword == "Sphere")
        {
            /* add sphere to hittable_list */
            point3 center;
            double radius = 0;
            Material material;
            // spere info is 8 lines .. hopefully consistently ..
            for (int i = 0; i < 8; i++)
            {
                std::getline(file, line);
                std::stringstream material_stream(line);
                std::string material_key;
                material_stream >> material_key;
                if (material_key == "Center")
                    material_stream >> center;
                else if (material_key == "Radius")
                    material_stream >> radius;
                else if (material_key == "Kd")
                    material_stream >> material.kd;
                else if (material_key == "Ks")
                    material_stream >> material.ks;
                else if (material_key == "Ka")
                    material_stream >> material.ka;
                else if (material_key == "Od")
                    material_stream >> material.od;
                else if (material_key == "Os")
                    material_stream >> material.os;
                else if (material_key == "Kgls")
                    material_stream >> material.kgls;
            }
            auto s = std::make_shared<sphere>(center, radius, material);
            scene.objects.add(s);
        }
    }
    return scene;
}