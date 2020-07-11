#include "assets.h"

#include "asset_load.h"

GPU_Image* Assets::invadersTexture;
GPU_Image* Assets::oldPlanetTexture;

GPU_Image* Assets::backgroundTextures[7];
GPU_Image* Assets::planetTextures[7];

GPU_Image* Assets::asterVoidTexture;

TTF_Font* Assets::font_30;
TTF_Font* Assets::font_30_outline;

Shader Assets::tintShader;

const std::string planets[7] = { 
    "data/planets/planet1.png", 
    "data/planets/planet2.png",
    "data/planets/planet3.png",
    "data/planets/planet4.png",
    "data/planets/planet5.png",
    "data/planets/planet6.png",
    "data/planets/planet7.png",
};

const std::string backgrounds[7] = {
    "data/bg/stars1.png",
    "data/bg/stars2.png",
    "data/bg/stars3.png",
    "data/bg/stars4.png",
    "data/bg/stars5.png",
    "data/bg/stars6.png",
    "data/bg/stars7.png",
};

void Assets::LoadAll() {
    invadersTexture = loadImage("data/invaders.png");
    oldPlanetTexture = loadImage("data/bg.png");

    for (int i = 0; i < size(planets); i++) {
        planetTextures[i] = loadImage(planets[i]);
    }

    for (int i = 0; i < size(backgrounds); i++) {
        backgroundTextures[i] = loadImage(backgrounds[i]);
        GPU_SetWrapMode(backgroundTextures[i], GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
    }

    asterVoidTexture = loadImage("data/astervoid.png");


    font_30 = loadFont("data/PressStart2P.ttf", 30);
    font_30_outline = loadFontOutline("data/PressStart2P.ttf", 30, 3);

    tintShader.Load("data/default.vert", nullptr, "data/tint.frag");
}
