#include <TextureGenerator.h>
#include <math.h>
#include <cstring>

using namespace std;

TextureGenerator::TextureGenerator(int w, int h)
    : width(w), height(h) {

    for (int i = 0; i < NUM_LAYERS; i++)
        layers[i] = new unsigned char[width * height * 4];
}

TextureGenerator::~TextureGenerator() {
    for (int i = 0; i < NUM_LAYERS; i++)
        if (layers[i] != nullptr)
            delete [] layers[i];
}

TextureGenerator& TextureGenerator::checkerBoard(unsigned char layer,
        unsigned char blockSize,
        Colour on,
        Colour off) {


    unsigned char* input = layers[layer];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (
                ((x % (2 * blockSize)) < blockSize) &&
                ((y % (2 * blockSize)) < blockSize)
            ) {
                *(reinterpret_cast<uint32_t*>(input)) = on.asInt();
                input += 4;
            } else {
                *(reinterpret_cast<uint32_t*>(input)) = off.asInt();
                input += 4;
            }
        }
    }

    return *this;
}


TextureGenerator& TextureGenerator::roll(unsigned char layer, unsigned char x, unsigned char y) {
    unsigned char* input = layers[layer];
    unsigned char* output = new unsigned char[width * height * 4];
    unsigned char* outptr = output;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
        	int index = ((j + y) % height) * width + ((i + x) % width);
            *outptr++ = input[index * 4 + 0];
            *outptr++ = input[index * 4 + 1];
            *outptr++ = input[index * 4 + 2];
            *outptr++ = input[index * 4 + 3];
        }
    }

    memcpy(input, output, width * height * 4);
    delete [] output;

    return *this;
}


unsigned char* TextureGenerator::get(unsigned char layer) {
    return layers[layer];
}

shared_ptr<Texture> TextureGenerator::getTexture(unsigned char layer) {
    return shared_ptr<Texture>(new Texture(width, height, get(layer)));
}

TextureGenerator& TextureGenerator::polarGrid(unsigned char layer) {
    unsigned char* input = layers[layer];

    unsigned char* output = new unsigned char[width * height * 4];
    unsigned char* outptr = output;

    for (int j = 0; j < height; j++) {
        double theta = M_PI * (j - (height - 1) / 2.0) / static_cast<double>(height - 1);
        for (int i = 0; i < width; i++) {
            double phi  = 2 * M_PI * (i - width / 2.0) / static_cast<double>(width);
            double phi2 = phi * cos(theta);
            int i2  = phi2 * width / (2 * M_PI) + width / 2;
            if (i2 < 0 || i2 > width - 1) {
                *outptr++ = 255;
                *outptr++ = 0;
                *outptr++ = 0;
                *outptr++ = 1;

            } else {
                *outptr++ = input[(j * width + i2) * 4 + 0];
                *outptr++ = input[(j * width + i2) * 4 + 1];
                *outptr++ = input[(j * width + i2) * 4 + 2];
                *outptr++ = input[(j * width + i2) * 4 + 3];
            }
        }
    }

    memcpy(input, output, width * height * 4);
    delete [] output;

    return *this;
}
