#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <Pixel.h>
#include <vector>
#define Min(a,b) ((a<b)?a:b)


class Texture
{
public:
    // Load a bitmap file
    void LoadFile(const std::string& filename)
    {
        //std::ifstream file;
        //file.open(filename.c_str());
        std::fstream file;
        file.open(filename, std::ios::in | std::ios::binary);
        if (file.is_open())
        {
            uint32_t sizeFile, offset;
            file.seekg(0, std::ios::beg);
            file.seekg(2);
            file.read((char*)&sizeFile, sizeof(sizeFile));
            file.seekg(10);
            file.read((char*)&offset, sizeof(offset));
            file.seekg(18);
            file.read((char*)&width, sizeof(width));
            file.seekg(22);
            file.read((char*)&height, sizeof(height));
            
            if (width > 0 && height > 0)
            {
                pixels.resize(width*height);
                file.seekg(offset);
                file.read((char*)&pixels[0], sizeof(ColorRGB)*width*height);
            }

            file.close();
        }
        else
            std::cout << "Couldn't load texture: " << filename << std::endl;
    }
    
    // Sample at given texture coordinates
    const ColorRGB& Sample(float u, float v) const
    {
        uint32_t x = uint32_t(u*((float)width-1));
        uint32_t y = uint32_t(v*((float)height-1));
        x = Min(x, width-1);
        y = Min(y, height-1);
        return pixels[y*width + x];
    }
    // Sample at given texture coordinates
    const ColorRGB& Sample(const Point2D& v) const
    {
        return Sample(v.x, v.y);
    }

    uint32_t width/*width of bitmap*/, height/*height of bitmap*/;
    std::vector<ColorRGB> pixels;   // array of pixels representing the image

};