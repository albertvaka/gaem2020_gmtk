#include "assets.h"

#include "asset_load.h"

GPU_Image* Assets::invadersTexture;
GPU_Image* Assets::oldPlanetTexture;

GPU_Image* Assets::backgroundTextures[7];
GPU_Image* Assets::planetTextures[7];

TTF_Font* Assets::font_30;
TTF_Font* Assets::font_30_outline;

Shader Assets::tintShader;

const std::string planets[7] = { 
    "planets/planet1.png", 
    "planets/planet2.png",
    "planets/planet3.png",
    "planets/planet4.png",
    "planets/planet5.png",
    "planets/planet6.png",
    "planets/planet7.png",
};

const std::string backgrounds[7] = {
    "bg/stars1.png",
    "bg/stars2.png",
    "bg/stars3.png",
    "bg/stars4.png",
    "bg/stars5.png",
    "bg/stars6.png",
    "bg/stars7.png",
};

void Assets::LoadAll() {
    invadersTexture = loadImage("data/invaders.png");
    oldPlanetTexture = loadImage("data/bg.png");

    for (int i = 0; i < size(planets); i++) {
        planetTextures[i] = loadImage(planets[i]);
        GPU_SetWrapMode(planetTextures[i], GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
    }

    for (int i = 0; i < size(backgrounds); i++) {
        backgroundTextures[i] = loadImage(planets[i]);
        GPU_SetWrapMode(backgroundTextures[i], GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
    }


    font_30 = loadFont("data/PressStart2P.ttf", 30);
    font_30_outline = loadFontOutline("data/PressStart2P.ttf", 30, 3);

    tintShader.Load("data/default.vert", nullptr, "data/tint.frag");
}
