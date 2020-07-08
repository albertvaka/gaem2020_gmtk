#include "assets.h"

#include "asset_load.h"

GPU_Image* Assets::invadersTexture;
GPU_Image* Assets::backgroundTexture;

TTF_Font* Assets::font_30;
TTF_Font* Assets::font_30_outline;

Shader Assets::tintShader;

void Assets::LoadAll() {

    invadersTexture = loadImage("data/invaders.png");
    backgroundTexture = loadImage("data/bg.png");

    font_30 = loadFont("data/PressStart2P.ttf", 30);
    font_30_outline = loadFontOutline("data/PressStart2P.ttf", 30, 3);

    tintShader.Load("data/default.vert", nullptr, "data/tint.frag");
}
