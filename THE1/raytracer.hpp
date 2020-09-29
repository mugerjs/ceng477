#ifndef RAYTRACER_HPP
#define  RAYTRACER_HPP

#include "scene.hpp"
#include "parser.h"

class RayTracer {
    static uygar::Scene *scene;
public:
    static void setScene(uygar::Scene *s);
    static void rayCast(parser::Vec3f source, parser::Vec3f destination, parser::Vec3f *intersectionPoint, uygar::SceneObject *intersectedObject);

    
};

#endif
