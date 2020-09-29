#include <iostream>
#include "scene.hpp"
#include "raytracer.hpp"

uygar::Scene* uygar::Scene::setData(parser::Scene scene) {
    std::vector<Vec3f*> vertices;
    std::for_each(scene.vertex_data.begin(),scene.vertex_data.end(),[&](Vec3f &vertex){vertices.push_back(&vertex);});
    std::vector<Material*> materials;
    std::for_each(scene.materials.begin(),scene.materials.end(),[&](Material &material){materials.push_back(&material);});

    uygar::Scene *s = new uygar::Scene();

    s->backgroundColor = scene.background_color;
    s->shadowRayEpsilon = scene.shadow_ray_epsilon;
    s->maxRecursionDepth = scene.max_recursion_depth;
    s->ambientLight = scene.ambient_light;
    s->pointLights = scene.point_lights;

    std::for_each(scene.meshes.begin(),scene.meshes.end(),[&](parser::Mesh m) {
        std::vector<uygar::Face* > faces;
        std::for_each(m.faces.begin(),m.faces.end(),[&](parser::Face &f){
            faces.push_back(new uygar::Face(vertices[f.v0_id - 1], vertices[f.v1_id - 1], vertices[f.v2_id - 1]));
        });
        s->objects.push_back(new Mesh(materials[m.material_id - 1], faces));
    });

    std::for_each(scene.triangles.begin(),scene.triangles.end(),[&](parser::Triangle t) {
        s->objects.push_back(new Triangle(materials[t.material_id - 1], new uygar::Face(vertices[t.indices.v0_id - 1], vertices[t.indices.v1_id - 1], vertices[t.indices.v2_id - 1])));
    });

    std::for_each(scene.spheres.begin(),scene.spheres.end(),[&](parser::Sphere sp) {
        s->objects.push_back(new Sphere(materials[sp.material_id - 1], vertices[sp.center_vertex_id - 1], sp.radius));
    });

    std::for_each(scene.cameras.begin(),scene.cameras.end(),[&](parser::Camera c){
        uygar::Camera camera(c);
        s->cameras.push_back(camera);
    });

    return s;
}

void uygar::Scene::exportImages() {
    RayTracer::setScene(this);
    //RayTracer::rayCast(new Vec3f(),new Vec3f(),new Vec3f(),NULL);
    //std::for_each(objects.begin(),objects.end(),[&](uygar::SceneObject *g){});

    std::for_each(cameras.begin(),cameras.end(),[](uygar::Camera camera){
        unsigned char* image = new unsigned char [camera.imageWidth * camera.imageHeight * 3];
        int resolution = camera.imageWidth * camera.imageHeight;


        for (int i = 0; i < resolution; i++) {
            uygar::SceneObject *intersectedObject = NULL;
            parser::Vec3f *intersectionPoint = NULL;
            parser::Vec3f m = camera.position - camera.w * camera.nearDistance;
            parser::Vec3f lu = camera.u * camera.nearPlane.x;
            parser::Vec3f tv = camera.v * camera.nearPlane.z;
            parser::Vec3f q = m + lu + tv;
            parser::Vec3f s = q + camera.u * (camera.nearPlane.y - camera.nearPlane.x) * ((i % camera.imageWidth) / camera.imageWidth) + camera.v * (camera.nearPlane.z - camera.nearPlane.w) * ((i / camera.imageWidth) / camera.imageHeight);

            RayTracer::rayCast(camera.position, s, intersectionPoint, intersectedObject);
            if (intersectedObject) {

                image[i/3] = intersectedObject->material->diffuse.x;
                image[i/3+1] = intersectedObject->material->diffuse.y;
                image[i/3+2] = intersectedObject->material->diffuse.z;
            }
            // else backgroundColor
        }

        write_ppm(camera.imageName.c_str(), image, camera.imageWidth, camera.imageHeight);
    });


}
