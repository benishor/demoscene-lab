#pragma once

#include <Colour.h>
#include <memory>
#include <Texture.h>

enum class RenderType {
    Wireframe,
    Solid
};

enum class ShadingType {
	Flat,
	Smooth
};

enum class OpacityType {
	Opaque,
	Transparent
};


struct RenderState {
	RenderType renderType;
	ShadingType shadingType;
	OpacityType opacityType;
	bool depthWrite;
	bool depthTest;
};


struct Material {
    Colour ambient;
    Colour diffuse;
    Colour specular;
    RenderType renderType;

    std::shared_ptr<Texture> texture;
};
