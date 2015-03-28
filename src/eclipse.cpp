#include <iostream>
#include <SDL2/SDL.h>
#include <SOIL/SOIL.h>

#include <Camera.h>
#include <Transform.h>
#include <Matrix.h>
#include <Vector.h>
#include <Rasterize.h>
#include <vector>
#include <texture.h>
#include <Sphere.h>

using namespace std;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const float def_Buffer = 100000;
Light light(1,1,1,1,1,1);

std::vector<Vertex> moonVertex;


int main()
{
    Camera camera;
    float lightAngle = 0;
    Texture texEarth, texMoon;
    texEarth.LoadFile("earthTexture.bmp");
    texMoon.LoadFile("moonTexture.bmp");
    
    Vec3 earthCenter = Vec3(0,0,500);
    float earthRadius = 175;
    Sphere earth(earthCenter, earthRadius, 25);
    Vec4 view_earthCenter;
    float earthAngle = 0;

    Vec3 moonCenter = Vec3(450,0,500);
    float moonRadius = 50;
    Sphere moon(moonCenter, moonRadius, 10);
    float moonAngle = 0;
    moonVertex.resize(moon.vertices.size());
    moonVertex[0].phi_count = moon.vertices[0].phi_count;
    moonVertex[0].theta_count = moon.vertices[0].theta_count;
    
	float zBuffer[(SCREEN_WIDTH+1)*(SCREEN_HEIGHT+1)];
    for(int i = 0; i<(SCREEN_WIDTH+1)*(SCREEN_HEIGHT+1);i++)
    {
        zBuffer[i] = def_Buffer;
    }
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout<<"cannot initialize"<<std::endl;
        return -1;
    }

    SDL_Window *window =SDL_CreateWindow("SOLAR_ECLIPSE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if(!window)
    return -1;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {

        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_Event event;
    bool quit = false;
   
    
    Mat4 projection, viewTransform, rotation, earthRotation;

    ColorRGB red(255,0,0), green(0,255,0), blue(0,0,255), black(0,0,0), grey(150,150,150), yellow(255,255,0), white(255,255,255);
    Line vAxis(renderer,0,-SCREEN_HEIGHT/2,0,SCREEN_HEIGHT/2, ColorRGB(255,0,0));
    Line hAxis(renderer,-SCREEN_WIDTH/2,0,SCREEN_WIDTH/2,0, ColorRGB(255,0,0));

       
    while(!quit)
    {        
        rotation = Transform::RotateZ(camera.gamma) * Transform::RotateX(camera.theta) * Transform::RotateY(camera.phi);
        view_earthCenter = Vec4(earthCenter, 1);

        // Changing light earth and moon position into viewing coordinate
        Vec4 lightDir = Vec4(light.direction, 0);
        lightDir = rotation * lightDir;
        light.direction = Vec3(lightDir.x, lightDir.y, lightDir.z);
    
        for(int i=0; i<earth.vertices.size(); i++)
        {
            Vec4 v = earth.vertices[i].position;
            Vec4 nearth = Vec4(earth.vertices[i].normal, 0);
            v = rotation * v;
            nearth = rotation * nearth;
            earth.vertices[i].position = v;
            earth.vertices[i].normal = Vec3(nearth.x, nearth.y, nearth.z);
        }
        for(int i=0; i<moon.vertices.size(); i++)
        {
            Vec4 v = moon.vertices[i].position;
            Vec4 nmoon = Vec4(moon.vertices[i].normal,0);
            v = rotation * v;
            nmoon = rotation * nmoon;
            moonVertex[i].position = moon.vertices[i].position = v;
            moon.vertices[i].normal = Vec3(nmoon.x, nmoon.y, nmoon.z);
        }

        //Rotation of Light source
        lightDir = Vec4(light.direction, 0);
        lightDir = Transform::RotateY(lightAngle) * lightDir;
        light.direction = Vec3(lightDir.x, lightDir.y, lightDir.z);
        lightAngle =0;
        
        //Rotation of earth
        for(int i=0; i<earth.vertices.size(); i++)
        {
            Vec4 v = earth.vertices[i].position;
            Vec4 nearth = Vec4(earth.vertices[i].normal, 0);
            v = Transform::RotateY(earthAngle, view_earthCenter) * v;
            nearth = Transform::RotateY(earthAngle, view_earthCenter) * nearth;
            earth.vertices[i].position = v;
            earth.vertices[i].normal = Vec3(nearth.x, nearth.y, nearth.z);
        }
        //Revolution of moon
        for(int i=0; i<moon.vertices.size(); i++)
        {
            Vec4 v = moon.vertices[i].position;
            Vec4 nmoon = Vec4(moon.vertices[i].normal,0);
            v = Transform::RotateZ(moonAngle, view_earthCenter) * v;
            nmoon = Transform::RotateZ(moonAngle, view_earthCenter) * nmoon;
            moonVertex[i].position = moon.vertices[i].position = v;
            moon.vertices[i].normal = Vec3(nmoon.x, nmoon.y, nmoon.z);
        }

        lightAngle =0;
        earthAngle = 0;
        moonAngle = 0;
        SDL_WaitEvent(&event);

        switch (event.type)
                {
                case SDL_QUIT:
                    quit = true;
                    break;

                    };
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_a) camera.phi =0.1;
            if (event.key.keysym.sym == SDLK_d) camera.phi =-0.1;
            if (event.key.keysym.sym == SDLK_q) camera.theta =0.1;
            if (event.key.keysym.sym == SDLK_e) camera.theta =-0.1;
            if (event.key.keysym.sym == SDLK_v) camera.gamma =0.1;
            if (event.key.keysym.sym == SDLK_b) camera.gamma =-0.1;
            if (event.key.keysym.sym == SDLK_m) { earthAngle = 0.01; moonAngle = 0.01; }
            if (event.key.keysym.sym == SDLK_n) { earthAngle = -0.01; moonAngle = -0.01; }
            if (event.key.keysym.sym == SDLK_j) lightAngle = 0.01;
            if (event.key.keysym.sym == SDLK_k) lightAngle = -0.01;
            
            if (event.key.keysym.sym == SDLK_z) {
                camera.zprp += 5;
                camera.zview += 5;
            }
            if (event.key.keysym.sym == SDLK_x) {
                camera.zprp -= 5;
                camera.zview -= 5;
            }





        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for(int i = 0; i<(SCREEN_WIDTH+1)*(SCREEN_HEIGHT+1);i++)
        {
            zBuffer[i] = def_Buffer;
        }

        
        
        // earth.Fill(renderer, camera, red, zBuffer);
        //sun.Fill(renderer, camera, yellow, zBuffer);
         earth.Map(renderer, camera, texEarth, zBuffer);
         moon.Map(renderer, camera, texMoon, zBuffer);

        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0; 
}