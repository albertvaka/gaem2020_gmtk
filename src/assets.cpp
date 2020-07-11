#include "assets.h"

#include "asset_load.h"

GPU_Image* Assets::invadersTexture;
GPU_Image* Assets::oldPlanetTexture;

GPU_Image* Assets::normalBackground;
GPU_Image* Assets::redBackground;
GPU_Image* Assets::blueBackground;

TTF_Font* Assets::font_30;
TTF_Font* Assets::font_30_outline;

Shader Assets::tintShader;

void Assets::LoadAll() {
    invadersTexture = loadImage("data/invaders.png");
    oldPlanetTexture = loadImage("data/bg.png");

    normalBackground = loadImage("data/bg/normal_bg.png");
    GPU_SetWrapMode(normalBackground, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
    redBackground = loadImage("data/bg/red_bg.png");
    GPU_SetWrapMode(redBackground, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
    blueBackground = loadImage("data/bg/blue_bg.png");
    GPU_SetWrapMode(blueBackground, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);

    font_30 = loadFont("data/PressStart2P.ttf", 30);
    font_30_outline = loadFontOutline("data/PressStart2P.ttf", 30, 3);

    tintShader.Load("data/default.vert", nullptr, "data/tint.frag");
}
