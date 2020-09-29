#include "raytracer.hpp"
#include <iostream>
#include <typeinfo>

uygar::Scene* RayTracer::scene = NULL;


void RayTracer::rayCast(parser::Vec3f source, parser::Vec3f destination, parser::Vec3f *intersectionPoint, uygar::SceneObject *intersectedObject) {
    uygar::SceneObject *io; //intersectedObject
    float ti = -1; // timeOfIntersection

    parser::Vec3f d = destination - source;
    parser::Vec3f o = source;

    auto D = [](parser::Vec3f v0, parser::Vec3f v1, parser::Vec3f v2) { //determinant function
        return v0.x*(v1.y * v2.z - v2.y * v1.z) +
        v0.y*(v1.z * v2.x - v2.z * v1.x) +
        v0.z*(v1.x * v2.y - v2.x * v1.y);
    };

    auto triangleIntersection = [&](uygar::Face *face) -> float { //returns flaot time of intersection return -1 if no intersection
        parser::Vec3f a = *(face->v0),
            b = *(face->v1),
            c = *(face->v2);

        parser::Vec3f a_o = a - o,
            a_b = a - b,
            a_c = a - c;

        float A = D(a_b,a_c,d);

        if (A <= 0)
            return -1;

        float beta = D(a_o,a_c,d),
            gamma = D(a_b,a_o,d),
            tmin = D(a_b,a_c,a_o);

        if (beta < 0 || gamma < 0 || beta + gamma > 1)
            return -1;

        return tmin;
    };

    std::for_each(scene->objects.begin(),scene->objects.end(),[&](uygar::SceneObject *object) {
        uygar::Triangle *triangle = dynamic_cast<uygar::Triangle*>(object);
        uygar::Sphere *sphere = dynamic_cast<uygar::Sphere*>(object);
        uygar::Mesh *mesh = dynamic_cast<uygar::Mesh*>(object);

        if (triangle) {
            int t = triangleIntersection(triangle->face);

            if ((ti == -1 || t < ti) && t > 0) {
                ti = t;
                io = object;
            }

            //std::cout << "triangle" << "\n";
        }
        else if (sphere) {
            parser::Vec3f c = *(sphere->center);
            float r = sphere->radius;

            parser::Vec3f o_c = (o-c);
            float d_sqrd = d*d;
            float dxo_c = d*(o_c);


            float b2_4ac = dxo_c*dxo_c - d_sqrd*(o_c*o_c - r*r);
            if (b2_4ac < 0)
                return;
            float t1 = (- dxo_c + b2_4ac) / d_sqrd;
            float t2 = (- dxo_c - b2_4ac) / d_sqrd;

            float tmin;
            if (t1 < 0 && t2 > 0)
                tmin = t2;
            else if (t2 < 0 && t1 > 0)
                tmin = t1;
            else if (t2 < 0 && t1 < 0)
                return;
            else
                tmin = t1 < t2 ? t1 : t2;

            if (ti == -1 || tmin < ti) {
                ti = tmin;
                io = object;
            }

            //std::cout << "sphere" << "\n";
        }
        else if (mesh) {
            std::for_each(mesh->faces.begin(),mesh->faces.end(),[&](uygar::Face *face) {
                int t = triangleIntersection(face);

                if ((ti == -1 || t < ti) && t > 0) {
                    ti = t;
                    io = object;
                }
            });

            //std::cout << "mesh" << "\n";
        }

    });

    //return ti io
    *intersectionPoint = d*ti + o;
    intersectedObject = io;

}

void RayTracer::setScene(uygar::Scene *s) {
    scene = s;
}
