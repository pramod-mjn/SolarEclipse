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

//Material material(0.5,0.5,0.5,200);




int main()
{
    Camera camera;
    Texture texEarth, texMoon;
    texEarth.LoadFile("earthTexture.bmp");
    texMoon.LoadFile("moonTexture.bmp");
    
    Vec3 earthCenter = Vec3(0,0,500);
    float earthRadius = 175;
    Sphere earth(earthCenter, earthRadius);
    Vec4 view_earthCenter;
    float earthAngle = 0;

    Vec3 moonCenter = Vec3(450,0,500);
    float moonRadius = 80;
    Sphere moon(moonCenter, moonRadius);
    float moonAngle = 0;

    Vec3 sunCenter = Vec3(-900,0,500);
    float sunRadius = 300;
    Sphere sun(sunCenter, sunRadius);
    Vec4 view_sunCenter;


    
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

    std::vector<Vec3> triangle = {Vec3(-50, -50, -50), Vec3(50, -50, -50), Vec3(-50, 50, -50), Vec3(50, 50, -50),
                                    Vec3(-50, -50, 50), Vec3(50, -50, 50), Vec3(-50, 50, 50), Vec3(50, 50,  50)};
    Vec3 grid[] = {Vec3(-100, 50, 300), Vec3(100, 50, 300), Vec3(-100, 50, -100), Vec3(100, 50, -100)};
    Vec4 view_grid[4];
    

    
    

       
    std::vector<Vec4> view_tran(triangle.size());
    bool quit = false;
   
    
    Mat4 projection, viewTransform, rotation, earthRotation;

    ColorRGB red(255,0,0), green(0,255,0), blue(0,0,255), black(0,0,0), grey(150,150,150), yellow(255,255,0), white(255,255,255);
    Line vAxis(renderer,0,-SCREEN_HEIGHT/2,0,SCREEN_HEIGHT/2, ColorRGB(255,0,0));
    Line hAxis(renderer,-SCREEN_WIDTH/2,0,SCREEN_WIDTH/2,0, ColorRGB(255,0,0));

       
    while(!quit)
    {
        
        moonAngle = 0.01;
        //vAxis.Draw();
        //hAxis.Draw();

        
       // viewTransform = Camera::ViewPort(camPos, Vec3(0,0,0), camAngle);
        rotation = Transform::RotateZ(camera.gamma) * Transform::RotateX(camera.theta) * Transform::RotateY(camera.phi);
        //earthRotation = Transform::RotateY(earthAngle, earthCenter);
        view_sunCenter = Vec4(sunCenter, 1);
        view_earthCenter = earthRotation * Vec4(earthCenter, 1);

        for(int i=0; i<triangle.size(); i++)
        {
        	view_tran[i] = rotation * Vec4(triangle[i], 1);
        }
        for(int i=0; i<4; i++)
        {
            view_grid[i] = rotation * Vec4(grid[i], 1);
            
        }
        for(int i=0; i<earth.vertices.size(); i++)
        {
            Vec4 v = earth.vertices[i].position;
            Vec4 nearth = Vec4(earth.vertices[i].normal, 0);
            //Vec4 v = Vec4(vec.x, vecc.y, vec.z, 1);
            v = Transform::RotateY(earthAngle, view_earthCenter) * v;
            nearth = Transform::RotateY(earthAngle, view_earthCenter) * nearth;
            earth.vertices[i].position = v;//Vec3(v.x, v.y, v.z);
            earth.vertices[i].normal = Vec3(nearth.x, nearth.y, nearth.z);
        }
        for(int i=0; i<moon.vertices.size(); i++)
        {
            Vec4 v = moon.vertices[i].position;
            Vec4 nmoon = Vec4(moon.vertices[i].normal,0);
            v = Transform::RotateZ(moonAngle, view_earthCenter) * v;
            nmoon = Transform::RotateZ(moonAngle, view_earthCenter) * nmoon;
            moon.vertices[i].position = v;//Vec3(v.x, v.y, v.z);
            moon.vertices[i].normal = Vec3(nmoon.x, nmoon.y, nmoon.z);
        }

        view_sunCenter = Vec4(sunCenter, 1);
        view_earthCenter = earthRotation * Vec4(earthCenter, 1);

        
        earthAngle = 0;
        SDL_WaitEvent(&event);

        switch (event.type)
                {
                case SDL_QUIT:
                    quit = true;
                    break;

                    };
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_a) camera.phi+=0.1;
            if (event.key.keysym.sym == SDLK_d) camera.phi-=0.1;
            if (event.key.keysym.sym == SDLK_q) camera.theta+=0.1;
            if (event.key.keysym.sym == SDLK_e) camera.theta-=0.1;
            if (event.key.keysym.sym == SDLK_v) camera.gamma+=0.1;
            if (event.key.keysym.sym == SDLK_b) camera.gamma-=0.1;
            if (event.key.keysym.sym == SDLK_m) earthAngle = 0.01;
            if (event.key.keysym.sym == SDLK_n) earthAngle = -0.01;
            
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

        
        //Rasterize(renderer, view_tran[0], view_tran[1], view_tran[2], zprp, zview, red).DrawTriangle();
        //Rasterize(renderer, view_tran[1], view_tran[2], view_tran[3], zprp, zview, red).DrawTriangle();
        
        // Rasterize(renderer, camera).FillTriangle(view_grid[0], view_grid[1], view_grid[2], grey, zBuffer);
        // Rasterize(renderer, camera).FillTriangle(view_grid[1], view_grid[2], view_grid[3], grey, zBuffer);
        
        
        // Rasterize(renderer, camera).FillTriangle(view_tran[0], view_tran[1], view_tran[2], red, zBuffer);
        // Rasterize(renderer, camera).FillTriangle(view_tran[1], view_tran[2], view_tran[3], red, zBuffer);
        // Rasterize(renderer, camera).FillTriangle(view_tran[4], view_tran[5], view_tran[6], black, zBuffer);
        // Rasterize(renderer, camera).FillTriangle(view_tran[5], view_tran[6], view_tran[7], black, zBuffer);
       
        // Rasterize(renderer, camera).FillTriangle(view_tran[0], view_tran[2], view_tran[4], green, zBuffer);
        // Rasterize(renderer, camera).FillTriangle(view_tran[2], view_tran[4], view_tran[6], green, zBuffer);
        // Rasterize(renderer, camera).FillTriangle(view_tran[1], view_tran[3], view_tran[5], green, zBuffer);
        // Rasterize(renderer, camera).FillTriangle(view_tran[3], view_tran[5], view_tran[7], green, zBuffer);
        
        // Rasterize(renderer, camera).FillTriangle(view_tran[0], view_tran[1], view_tran[5], blue, zBuffer);
        // Rasterize(renderer, camera).FillTriangle(view_tran[0], view_tran[5], view_tran[4], blue, zBuffer);
        // Rasterize(renderer, camera).FillTriangle(view_tran[2], view_tran[3], view_tran[7], blue, zBuffer);
        // Rasterize(renderer, camera).FillTriangle(view_tran[2], view_tran[6], view_tran[7], blue, zBuffer);
        

        // Rasterize(renderer, camera).FillTriangle(view_tran[4], view_tran[5], view_tran[6], black, zBuffer);
        // Rasterize(renderer, camera).FillTriangle(view_tran[5], view_tran[6], view_tran[7], black, zBuffer);
        
        // Rasterize(renderer, camera).FillSphere(view_sunCenter, sunRadius, texEarth, zBuffer);
        // Rasterize(renderer, camera).DrawSphere(view_earthCenter, earthRadius, blue);
        //earth.Fill(renderer, camera, red, zBuffer);
        //sun.Fill(renderer, camera, yellow, zBuffer);
        earth.Map(renderer, camera, texEarth, zBuffer);
        moon.Map(renderer, camera, texMoon, zBuffer);



        
        //vAxis.Draw();
        //hAxis.Draw();
       
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0; 
}