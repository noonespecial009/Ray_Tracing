#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "surface_texture.h"
vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) { 
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
             return attenuation*color(scattered, world, depth+1);
        }
        else {
            return vec3(0,0,0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}
hitable *earth() {
    
    int nx, ny, nn;
    //unsigned char *tex_data = stbi_load("tiled.jpg", &nx, &ny, &nn, 0);
    unsigned char *tex_data = stbi_load("cabin.jpg", &nx, &ny, &nn, 1);
    
    hitable **list = new hitable*[2];

    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(new constant_texture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
    list[1] = new sphere(vec3(2,2,0.0), 2, new lambertian(new image_texture(tex_data, nx, ny)));
    return new hitable_list(list,2);
    

}
hitable *earth2() {
    int nx, ny, nn;
    //hitable **list = new hitable*[2];
    //unsigned char *tex_data = stbi_load("tiled.jpg", &nx, &ny, &nn, 0);
    unsigned char *tex_data = stbi_load("bat.jpg", &nx, &ny, &nn, 0);
    material *mat =  new lambertian(new image_texture(tex_data, nx, ny));
    return new sphere(vec3(0,0, 0), 2, mat);
}
hitable *ballChecker(){ //ball4.ppm
    hitable **list = new hitable*[2];
    texture *checker = new checker_texture( new constant_texture(vec3(0.2,0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian( checker));
    list[1] = new sphere(vec3(2,1,0.0), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
    return new hitable_list(list,2);
}
hitable *areaLight(){ //ball4.ppm
    hitable **list = new hitable*[2];
    texture *checker = new checker_texture( new constant_texture(vec3(0.2,0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian( checker));
    list[1] = new sphere(vec3(2,1,0.0), 0.5, new diffuse_light(new constant_texture(vec3(4,4,4))));
    return new hitable_list(list,2);
}
hitable *ballPlain(){ //ball6.ppm, ball7
    hitable **list = new hitable*[3];
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(new constant_texture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
    list[1] = new sphere(vec3(1,1,-1), 0.75, new lambertian(new constant_texture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
      list[2] = new sphere(vec3(-1,0.5,2), 0.75, new lambertian(new constant_texture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
    return new hitable_list(list,3);
}
hitable *ballGlass(){ //ball8.ppm
    hitable **list = new hitable*[6];
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(new constant_texture(vec3(0.8, 0.8, 0.0))));
    //list[1] = new sphere(vec3(-3,1,1), -0.75, new dielectric(1.5));
    list[1] = new sphere(vec3(2, 1.2, 1), 1.1, new dielectric(1.5));
    list[2] = new sphere(vec3(1, 0.9, -2), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    list[3] = new sphere(vec3(1, 0.9, -2), 1.1, new dielectric(1.5));
    list[4] = new sphere(vec3(5,0.5,0.0), 0.25, new lambertian(new constant_texture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
    list[5] = new sphere(vec3(5.0,0.35,2.5), 0.25, new lambertian(new constant_texture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
    return new hitable_list(list,6);
}
hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];
    texture *checker = new checker_texture( new constant_texture(vec3(0.2,0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian( checker));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48()); 
            if ((center-vec3(4,0.2,0)).length() > 0.9) { 
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new moving_sphere(center, center+vec3(0,0.5*drand48(), 0), 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
                }
                else if (choose_mat < 0.95) { // metal
                    list[i++] = new sphere(center, 0.2,
                            new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())),  0.5*drand48()));
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list,i);
}

int main() {
    int nx = 1200;
    int ny = 800;
    int ns = 10;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    hitable *list[5];
    float R = cos(M_PI/4);
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(new constant_texture(vec3(0.1, 0.2, 0.5))));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(new constant_texture(vec3(0.8, 0.8, 0.0))));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(list,5);
    world = areaLight();

    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;

    camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);

    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s=0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world,0);
            }
            col /= float(ns);
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            int ir = int(255.99*col[0]); 
            int ig = int(255.99*col[1]); 
            int ib = int(255.99*col[2]); 
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}



