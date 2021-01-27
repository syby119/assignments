#pragma once


#include "../scene/scene.h"

class Renderer {
public:
    virtual void render(const Scene& scene, ) = 0;
// private:
//     Framebuffer _framebuffer;
private:
    uint32_t 
    int _xResolution = 0;
    int _yResolution = 0;
};
