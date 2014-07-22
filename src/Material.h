#pragma once

#include <Colour.h>
#include <memory>
#include <Texture.h>
#include <Shader.h>

enum class FillMode {
    Wireframe,
    Solid
};

enum class BlendingConstant {
    SRC_COLOR,
    DST_COLOR,
    ONE,
    ZERO,
    SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    ONE_MINUS_DST_ALPHA,
};


struct Material {

    Colour ambient;
    Colour diffuse;
    Colour specular;
    int shininess = 100;

    bool isFlatShaded = true;
    bool shouldWriteToDepthBuffer = true;
    bool shouldTestDepthBuffer = true;
    bool shouldCullFaces = false;
    bool shouldCullFrontFaces = false;
    bool isOpaque = true; // if false, blending is enabled

    BlendingConstant blendingSourceConstant = BlendingConstant::SRC_ALPHA;
    BlendingConstant blendingDestinationConstant = BlendingConstant::ONE_MINUS_SRC_ALPHA;

    FillMode fillMode = FillMode::Solid;

    std::shared_ptr<Texture> texture;
    std::shared_ptr<Shader> shader;

    void use() const;
    void unuse() const;
};
