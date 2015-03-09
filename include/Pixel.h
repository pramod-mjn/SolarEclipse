#ifndef PIXEL
#define PIXEL

#include <SDL2/SDL.h>
#include <Vector.h>

const int WIDTH = 1024;
const int HEIGHT = 768;

// class for color
struct ColorRGB
{
		ColorRGB()
		{
			R=255;G=255;B=255;
		}
        ColorRGB(uint8_t b, uint8_t g, int8_t r) : R(r), G(g), B(b) {}
        uint8_t R, G, B;
};

// plotting pixel
inline void setPixel(SDL_Renderer *renderer, int x, int y, ColorRGB color)
{
        x +=WIDTH/2; y +=HEIGHT/2;
        if (x <0  || x >= WIDTH || y < 0 || y >= HEIGHT) return;
        SDL_Rect pixel = {x, y, 1, 1};
        SDL_SetRenderDrawColor(renderer, color.B, color.G, color.R, 0xFF);
        SDL_RenderFillRect(renderer, &pixel);
}
inline void setPixel(SDL_Renderer *renderer, int x, int y, uint32_t color)
{
    uint8_t r = (color >> 16) & 0x0000ff;
    uint8_t g = (color >> 8)  & 0x0000ff;
    uint8_t b = (color)       & 0x0000ff;
    SDL_Rect pixel = {x+WIDTH/2, y+HEIGHT/2, 1, 1};
    SDL_SetRenderDrawColor(renderer, r, g, b, 0);
    SDL_RenderFillRect(renderer, &pixel);
}

inline void setPixel(SDL_Renderer * renderer, Point2D p, ColorRGB color)
{
	SDL_Rect pixel = {p.x+WIDTH/2, p.y+HEIGHT/2, 1,1};
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, 0xFF);
    SDL_RenderFillRect(renderer, &pixel);
}
//colors
extern ColorRGB RED;
extern ColorRGB GREEN;
extern ColorRGB BLUE;
extern ColorRGB WHITE;
extern ColorRGB BLACK;
/*
ColorRGB LIGHT_RED(250, 160, 122, 255);
ColorRGB CRIMSON(220, 20, 60, 255);
ColorRGB DARK_RED(139, 0, 0, 255);
ColorRGB ORANGE(255, 69, 0, 255);
ColorRGB YELLOW(255, 255, 0, 255);
ColorRGB GOLD(255, 215, 0, 255);
ColorRGB MAROON(128, 0, 0, 255);
ColorRGB SKY_BLUE(135, 206,235, 255);
*/

#endif
