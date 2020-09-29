#ifndef SCENE_HPP
#define SCENE_HPP

#include "parser.h"
#include "ppm.h"
#include <algorithm>

using namespace parser;

namespace uygar {
    class SceneObject {
    public:
        Material *material;
        SceneObject(Material *m) : material(m) {}
        virtual ~SceneObject() {}
    };

    class Camera {
    public:
        Camera(parser::Camera c) : position(c.position), gaze(c.gaze),
        up(c.up), nearPlane(c.near_plane), nearDistance(c.near_distance),
        imageWidth(c.image_width), imageHeight(c.image_height), imageName(c.image_name) {
            v = up;
            w = gaze * -1;
            u = Vec3f::cross(v,w);
        };
        Vec3f position;
        Vec3f gaze;
        Vec3f up;
        Vec4f nearPlane;
        float nearDistance;
        int imageWidth, imageHeight;
        std::string imageName;
        Vec3f u,v,w;
    };

    class Scene {
    public:
        parser::Vec3i backgroundColor;
        float shadowRayEpsilon;
        int maxRecursionDepth;
        parser::Vec3f ambientLight;
        std::vector<PointLight> pointLights;
        std::vector<uygar::Camera> cameras;

        std::vector<SceneObject*> objects;
        static uygar::Scene* setData(parser::Scene scene);
        void exportImages();
    };

    class Face {
    public:
        Vec3f *v0,*v1,*v2;
        Face (Vec3f *x, Vec3f *y, Vec3f *z): v0(x), v1(y), v2(z) {}
    };

    class Triangle : public SceneObject {
    public:
        Face *face;
        Triangle (Material *m, Face *f): SceneObject(m), face(f) {}
    };

    class Sphere : public SceneObject {
    public:
        Vec3f *center;
        float radius;
        Sphere (Material *m, Vec3f *c, float r): SceneObject(m), center(c), radius(r) {}
    };

    class Mesh : public SceneObject {
    public:
        std::vector<Face*> faces;
        Mesh (Material *m, std::vector<Face*> f): SceneObject(m), faces(f) {}
    };
}

#endif
