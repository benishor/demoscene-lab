#pragma once

#include <Scene.h>
#include <Camera.h>

class Renderer {
public:
	
    void render(const Scene& scene, Camera& camera);
};