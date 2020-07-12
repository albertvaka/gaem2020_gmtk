#include "assets.h"

#include "asset_load.h"

namespace Assets
{
    const std::string planets[7] = {
        "data/planet1.png",
        "data/planet2.png",
        "data/planet3.png",
        "data/planet4.png",
        "data/planet5.png",
        "data/planet6.png",
        "data/planet7.png",
    };

    const std::string backgrounds[3] = {
        "data/stars2.png",
        "data/stars4.png",
        "data/stars6.png",
    };

    void LoadAll() {
        ship1Texture = loadImage("data/ship1.png");
        ship2Texture = loadImage("data/ship2.png");

        for (int i = 0; i < size(planets); i++) {
            planetTextures[i] = loadImage(planets[i]);
        }

        for (int i = 0; i < size(backgrounds); i++) {
            backgroundTextures[i] = loadImage(backgrounds[i]);
            GPU_SetWrapMode(backgroundTextures[i], GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
        }

        asterVoidTexture = loadImage("data/astervoid.png");
        solTexture = loadImage("data/blackpixel.png");

        font_30 = loadFont("data/PressStart2P.ttf", 30);
        font_30_outline = loadFontOutline("data/PressStart2P.ttf", 30, 3);

        shot_small[0].Load("data/shot_small1.wav");
        shot_small[1].Load("data/shot_small2.wav");
        shot_small[2].Load("data/shot_small3.wav");
        shot_medium[0].Load("data/shot_medium1.wav");
        shot_medium[1].Load("data/shot_medium2.wav");
        shot_large[0].Load("data/shot_large1.wav");
        hit.Load("data/hit.wav");
        shield.Load("data/shield.wav");
    }

}