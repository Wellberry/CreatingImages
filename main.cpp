#include <iostream>
#include <fstream>
#include <math.h>

struct Vec{
    float x = 0, y = 0, z = 0;
    Vec(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vec(){}
};
struct Vector{
    Vec pos, dir;
    Vector(float posX, float posY, float posZ,
            float dirX, float dirY, float dirZ){
        pos.x = posX;
        pos.y = posY;
        pos.z = posZ;
        dir.x = dirX;
        dir.y = dirY;
        dir.z = dirZ;
    }
    Vector(Vec pos,
            float dirX, float dirY, float dirZ){
        this->pos = pos;
        dir.x = dirX;
        dir.y = dirY;
        dir.z = dirZ;
    }
    float getSquareAbs(){
        return dir.x*dir.x + dir.y*dir.y + dir.z*dir.z;
    }
};

float dot(Vector a, Vector b){
    return a.dir.x*b.dir.x+a.dir.y*b.dir.y+a.dir.z*b.dir.z;
}

Vector sum(Vector a, Vector b){
    Vector res(0, 0, 0, 0, 0, 0);
    res.pos = a.pos;
    res.dir.x = a.dir.x + b.dir.x;
    res.dir.y = a.dir.y + b.dir.y;
    res.dir.z = a.dir.z + b.dir.z;
    return res;
}

Vector scalar(Vector a, float scal){
    a.dir.x*=scal;
    a.dir.y*=scal;
    a.dir.z*=scal;
    return a;
}

Vector addToLength(Vector a, float distance){
    float Dist = distance + sqrt(a.getSquareAbs());
    Vector res = scalar(a, Dist/sqrt(a.getSquareAbs()));
    return a;
}

const int w=250, h=250;
int* screen = new int[w*h*3];
Vector light(0, 0, 0, 1, 1, -2);
Vector camera(0, 0, -30, 0, 0, 40);
Vector Sphere(0, 0, 0, 0, 0, 0);
float R = 1;

float dist(Vec point){
    float X = Sphere.pos.x, Y = Sphere.pos.y, Z = Sphere.pos.z;
    float x = point.x, y = point.y, z = point.z;
    return sqrt((x-X)*(x-X)+(y-Y)*(y-Y)+(z-Z)*(z-Z))-R;
}

int check(int deep, Vector ray){
    float distacne = dist(ray.dir);
    int color = 0;
    if(deep > 15){
        return color;
    }else if(distacne<100){
        Vector sphere = sum(Sphere, scalar(ray, -1.0));
        int color = (255*dot(light, sphere)/sqrt(ray.getSquareAbs()));
        // if (color>255)color=255;
        if(color<0)color=0;
        return color;
    } else {
        Vec point(ray.pos.x+ray.dir.x,ray.pos.y+ray.dir.y,ray.pos.z+ray.dir.z);
        int color = check(deep+1, addToLength(ray, dist(point)));
        return color;
    }
}

int rayMarching(int i, int j){
    Vector x(0, 0, 0, i, 0, 0);
    Vector y(0, 0, 0, 0, j, 0);
    Vector ray = sum(sum(camera, x), y);
    int color = check(0, ray);
    return color;
}

int main(){

    light = scalar(light, 1.0f/sqrt(light.getSquareAbs()));
    // std::cout<<sqrt(light.getSquareAbs())<<"\n";

    for (int y=0; y<h; y++){
        for (int x = 0; x<w; x++){
            int color = rayMarching(x-w/2, y-h/2);
            if((y<h*0.4 | y>h*0.5) | (x<20 | x>w-20)){
            screen[(y*h+x)*3+0] = color;
            screen[(y*h+x)*3+1] = color;
            screen[(y*h+x)*3+2] = color;
            }else{
            screen[(y*h+x-20)*3+0] = color;
            screen[(y*h+x)*3+1] = color;
            screen[(y*h+x+20)*3+2] = color;
            }
        }
    }

    std::ofstream image;
    image.open("image.ppm");
    if(image.is_open()){
        image << "P3" << std::endl;
        image << std::to_string(w)<< " "<< std::to_string(h) << std::endl;
        image << "255" << std::endl;

        for (int i = 0; i < h; i++){
            for (int j = 0; j < w; j++){
                image << std::to_string(screen[(i*h+j)*3+0])<<" ";
                image << std::to_string(screen[(i*h+j)*3+1])<<" ";
                image << std::to_string(screen[(i*h+j)*3+2])<< std::endl;
            }
        }
    }
    image.close();
    delete[] screen;

    return 0;
}