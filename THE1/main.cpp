#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "scene.hpp"

typedef unsigned char RGB[3];

int main(int argc, char* argv[]) {
    parser::Scene scene;

    scene.loadFromXml(argv[1]);

    parser::Vec3f aa = parser::Vec3f(3,3,3) - parser::Vec3f(2,2,2);
    parser::Vec3f as = parser::Vec3f(3,3,3) + parser::Vec3f(2,2,2);
    float ad = parser::Vec3f(3,3,3) * parser::Vec3f(2,2,2);
    parser::Vec3f af = parser::Vec3f(3,3,3) * 2.0;


    parser::Vec3f ct = parser::Vec3f::cross(parser::Vec3f(1,2,3), parser::Vec3f(4,8,7));


    uygar::Scene::setData(scene)->exportImages();
}
