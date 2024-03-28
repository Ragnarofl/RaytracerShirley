#include <iostream>
#include <algorithm>
#include "Random_unit.h"
#include "Random.h"
#include "Vec3.h"
#include "Camera.h"
#include "Ray.h"
#include "Sphere.h"
#include "HitableList.h"
#include "easyppm.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"
#include "Rectangle.h"
#include "Diffuse_light.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "BvhNode.h"
#include "FlipNormals.h"
#include "Box.h"
#include "Translation.h"
#include "Rotation.h"
#include "ConstantMedium.h"

AABB surrounding_box(AABB box0, AABB box1) {
    vec3 small(fmin(box0.min().x(), box1.min().x()),
        fmin(box0.min().y(), box1.min().y()),
        fmin(box0.min().z(), box1.min().z()));
    vec3 big(fmax(box0.max().x(), box1.max().x()),
        fmax(box0.max().y(), box1.max().y()),
        fmax(box0.max().z(), box1.max().z()));
    return AABB(small, big);
}

ppmcolor switch_int(vec3 col)
{
    ppmcolor c; //switch back to integer
    c.r = static_cast<int>(255.99 * col.x());
    c.g = static_cast<int>(255.99 * col.y());
    c.b = static_cast<int>(255.99 * col.z());

    return c;
}

vec3 color(Ray& r, BvhNode* world, int depth, std::mt19937 mt)
{
    //Ray hit data
    hit_record rec;

    if (world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec, mt)) {
        Ray scattered; //New ray
        vec3 attenuation; //Object albedo
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p); //Light emitted from the object;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered, mt))
            return emitted + attenuation * color(scattered, world, depth + 1, mt); //We add light and color contribution
        else
            return emitted;
    }
    else {
        //vec3 unitDirection = unit_vector(r.direction());
        //float t = 0.5 * (unitDirection.y() + 1.0);
        //return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
        return vec3(0, 0, 0);
    }
}

vec3 color(Ray& r, Hitable* world, int depth, std::mt19937 mt, vec3 pointLight)
{
    //Ray hit data
    hit_record rec;

    if (world->hit(r, 0.01f, std::numeric_limits<float>::max(), rec, mt)) {
        Ray scattered; //New ray
        vec3 attenuation; //Object albedo
        //vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p); //Light emitted from the object;
        if (depth < 20 && rec.mat_ptr->scatter(r, rec, attenuation, scattered, mt)) {
            Ray shadowRay(rec.p + rec.normal * 0.01, pointLight - rec.p); //Create a shadow ray to test light visibility with a bias to include self collision
            if (!world->hit(shadowRay, 0.01f, std::numeric_limits<float>::max(), rec, mt)) //Test if light is visible
                return attenuation; // color(scattered, world, depth + 1); //We add light and color contribution
            else
                return vec3(0, 0, 0); // color(scattered, world, depth + 1);
        }
    }
    else
        return vec3(0, 0, 0);
}

BvhNode* RandomScene()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    int n = 500;
    Hitable** list = new Hitable * [n + 1];
    list[0] = new Sphere(vec3(0, -1000, 0), 1000.0f, new Lambertian(new Constant_texture(vec3(0.5, 0.5, 0.5))));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random_unit_float(gen);
            vec3 center(a + 0.9 * random_unit_float(gen), 0.2, b + 0.9 * random_unit_float(gen));
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    list[i++] = new Sphere(center, 0.2f, new Lambertian(new Constant_texture(vec3(random_unit_float(gen) * random_unit_float(gen),
                        random_unit_float(gen) * random_unit_float(gen),
                        random_unit_float(gen) * random_unit_float(gen)))));
                }
                else if (choose_mat < 0.95) {
                    list[i++] = new Sphere(center, 0.2f,
                        new Metal(vec3(0.5 * (1 + random_unit_float(gen)), 0.5 * (1 + random_unit_float(gen)), 0.5 * (1 + random_unit_float(gen))), 0.5 * 0.5 * (1 + random_unit_float(gen))));
                }
                else {
                    list[i++] = new Sphere(center, 0.2f, new Dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new Sphere(vec3(0, 1, 0), 1.0f, new Dielectric(1.5));
    list[i++] = new Sphere(vec3(-2.5, 1, 0), 1.0f, new Lambertian(new Constant_texture(vec3(0.4, 0.2, 0.1))));
    list[i++] = new Sphere(vec3(2.5, 1, 0), 1.0f, new Metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new BvhNode(list, i, 0.0f, 0.0f, gen);
    /* Camera Params
    vec3 lookFrom(0, 1.5, 7);
    vec3 lookAt(0, 0, 0);
    float dist_to_focus = (lookFrom - lookAt).length();
    float aperture = 0.03f;
    float vfov = 40.0f;
    */
}

BvhNode* CornellBox()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    Hitable** list = new Hitable * [8];
    int i = 0;
    Material* red = new Lambertian(new Constant_texture(vec3(0.65, 0.05, 0.05)));
    Material* white = new Lambertian(new Constant_texture(vec3(0.73, 0.73, 0.73)));
    Material* green = new Lambertian(new Constant_texture(vec3(0.12, 0.45, 0.15)));
    Material* light = new Diffuse_light(new Constant_texture(vec3(15, 15, 15)));
    list[i++] = new FlipNormals(new yz_rect(0, 555, 0, 555, 555, green)); //left wall, flipped normal so it's inside
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red); //right wall
    list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
    list[i++] = new FlipNormals(new xz_rect(0, 555, 0, 555, 555, white)); //top wall
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white); //bottom wall
    list[i++] = new FlipNormals(new xy_rect(0, 555, 0, 555, 555, white)); //back wall
    Hitable* b1 = new Translation(vec3(130, 0, 65), new RotateY(-18.0f, new Box(vec3(0, 0, 0), vec3(165, 165, 165), white)));
    Hitable *b2 = new Translation(vec3(265, 0, 295), new RotateY(15.0f, new Box(vec3(0, 0, 0), vec3(165, 330, 165), white)));
    list[i++] = new ConstantMedium(b1, 0.01, new Constant_texture(vec3(0.0, 0.0, 0.0)));
    list[i++] = new ConstantMedium(b2, 0.05, new Constant_texture(vec3(0, 0, 0)));

    Hitable* boundary = new Sphere(vec3(330, 330, 380), 50.0f, new Lambertian(new Constant_texture(vec3(1.0, 0.0, 0.0))));
    //list[i++] = boundary;
    list[i++] = new ConstantMedium(boundary, 0.2, new Constant_texture(vec3(0.2, 0.4, 0.9)));


    return new BvhNode(list, i, 0.0f, 0.0f, gen);
    /* Camera Params
    vec3 lookFrom(278, 278, -800);
    vec3 lookAt(278, 278, 0);
    float dist_to_focus = (lookFrom - lookAt).length();
    float aperture = 0.0f;
    float vfov = 40.0f;
    */
}

BvhNode* FinalScene()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    int nb = 20;
    Hitable** list = new Hitable * [30];
    Hitable** boxList = new Hitable * [10000];
    Hitable** boxList2 = new Hitable * [10000];
    Material* white = new Lambertian(new Constant_texture(vec3(0.73, 0.73, 0.73)));
    Material* ground = new Lambertian(new Constant_texture(vec3(0.48, 0.83, 0.53)));
    int b = 0;
    for (int i = 0; i < nb; i++)
        for (int j = 0; j < nb; j++) {
            float w = 100;
            float x0 = -1000 + i * w;
            float y0 = 0;
            float z0 = -1000 + j * w;
            float x1 = x0 + w;
            float y1 = 100 * (random_unit_float(gen) + 0.01);
            float z1 = z0 + w;
            boxList[b++] = new Box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
        }
    int l = 0;
    list[l++] = new BvhNode(boxList, b, 0, 1, gen);
    list[l++] = new xz_rect(123, 423, 147, 412, 554, new Diffuse_light(new Constant_texture(vec3(7, 7, 7))));
    //vec3 center(400, 400, 200);
    //list[l++] = new MovingSphere(center, center + vec(30, 0, 0), 0, 1, 50, new Lambertian(new Constant_texture(vec3(0.7, 0.3, 0.1))));
    list[l++] = new Sphere(vec3(260, 150, 45), 50.0f, new Dielectric(1.5));
    list[l++] = new Sphere(vec3(0, 150, 145), 50.0f, new Metal(vec3(0.8, 0.8, 0.9), 10.0f));
    Hitable* boundary = new Sphere(vec3(360, 150, 145), 70.0f, new Dielectric(1.5));
    list[l++] = boundary;
    list[l++] = new ConstantMedium(boundary, 0.2, new Constant_texture(vec3(0.2, 0.4, 0.9)));
    boundary = new Sphere(vec3(0, 0, 0), 5000, new Dielectric(1.5));
    list[l++] = new ConstantMedium(boundary, 0.0001, new Constant_texture(vec3(1.0, 1.0, 1.0)));
    int nxx, nyy, nn;
    unsigned char* tex_data = stbi_load("earthmap.jpg", &nxx, &nyy, &nn, 0);
    list[l++] = new Sphere(vec3(400, 200, 400), 100.0f, new Lambertian(new Image_texture(tex_data, nxx, nyy)));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxList2[j] = new Sphere(vec3(165 * random_unit_float(gen), 165 * random_unit_float(gen), 165 * random_unit_float(gen)), 10.0f, white);
    }
    list[l++] = new Translation(vec3(-100, 270, 395), new RotateY(15, new BvhNode(boxList2, ns, 0.0f, 1.0f, gen)));
    return new BvhNode(list, l, 0.0f, 1.0f, gen);
    /* Camera Params
    vec3 lookFrom(600, 278, -800);
    vec3 lookAt(278, 278, 0);
    float dist_to_focus = (lookFrom - lookAt).length();
    float aperture = 0.0f;
    float vfov = 40.0f;*/
}

int main()
{
    //window size
    int nx = 800;
    int ny = 800;
    //sampling size
    int ns = 100;
    //Image Settings
    int nxx, nyy, nn;
    unsigned char* tex_data = stbi_load("earthmap.jpg", &nxx, &nyy, &nn, 0);

    //Camera setting
    /*vec3 lookFrom(26, 3, 6);
    vec3 lookAt(0, 2, 0);
    float dist_to_focus = (lookFrom - lookAt).length();
    float aperture = 0.0f;
    float vfov = 20.0f;*/
    vec3 lookFrom(278, 278, -800);
    vec3 lookAt(278, 278, 0);
    float dist_to_focus = (lookFrom - lookAt).length();
    float aperture = 0.0f;
    float vfov = 40.0f;
    Camera cam(lookFrom, lookAt, vec3(0, 1, 0), vfov, static_cast<float>(nx) / static_cast<float>(ny), aperture, dist_to_focus);

    Texture* checker = new Checker_texture(new Constant_texture(vec3(0.2, 0.3, 0.1)), new Constant_texture(vec3(0.9, 0.9, 0.9)));

    Hitable* list[4];
    Hitable* boundary = new Sphere(vec3(0, 2.5, 0), 2.0f, new Dielectric(1.5f));
    list[0] = boundary;
    list[1] = new Sphere(vec3(0, -100.5, -1.0), 100.0f, new Lambertian(checker));
    list[2] = new Sphere(vec3(0, 7, 0), 1.5f, new Diffuse_light(new Constant_texture(vec3(4, 4, 4))));
    list[3] = new ConstantMedium(boundary, 0.2, new Constant_texture(vec3(0.2, 0.4, 0.9)));

    //BvhNode* tree = new BvhNode(list, 4, 0.0f, 0.0f, mt); // we give the time frame in case of moving objects
    //BvhNode* tree = RandomScene();
    BvhNode* tree = CornellBox();
    //BvhNode* tree = FinalScene();

    //vec3 pointLight(0, 7, 0);

    PPM ppm = easyppm_create(nx, ny, IMAGETYPE_PPM); // Image size setup, bottom-left origin

    #pragma omp parallel num_threads(16)
    {
        // Create a random seed for each thread
        std::random_device seed;
        std::mt19937 mt(seed());

        // Split the following loop into multiple threads. Each thread will take a designated chunk of itterations
        // to avoid creating threads a lot of times
        int count = 1;
        #pragma omp for schedule(dynamic, 50)
        for (int j = ny - 1; j >= 0; j--) {
            for (int i = 0; i < nx; i++) {
                vec3 sum(0.0f, 0.0f, 0.0f); //sum for kahan-summation
                vec3 c(0.0f, 0.0f, 0.0f); //error storing variable
                for (int s = 0; s < ns; s++) { //Random sampling for Anti-Aliasing
                    float u = static_cast<float>(i + random_unit_float(mt)) / static_cast<float>(nx);
                    float v = static_cast<float>(j + random_unit_float(mt)) / static_cast<float>(ny);

                    Ray r = cam.get_ray(u, v, mt);
                    //kahan-summation to reduce float error
                    vec3 y = color(r, tree, 0, mt) - c;
                    vec3 t = sum + y;
                    c = (t - sum) - y;
                    sum = t;
                }
                vec3 col = sum / static_cast<float>(ns); //get the average color
                col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2])); //Gamma correction
                easyppm_set(&ppm, i, j, switch_int(col));
            }
            if (count == 25)
                std::cout << "Thread " << omp_get_thread_num() << " finished 25 columns\n";
            if (count == 50)
                std::cout << "Thread " << omp_get_thread_num() << " finished 50 columns\n";
            count++;
        }
    }

    easyppm_invert_y(&ppm);
    easyppm_write(&ppm, "../Results/result_test1.ppm");
}
