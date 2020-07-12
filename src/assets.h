#pragma once

#include <array>

#include "sound.h"
#include "shader.h"

struct GPU_Image;
typedef struct _TTF_Font TTF_Font;
typedef struct _Mix_Music Mix_Music;

struct Assets
{
	static void LoadAll();

	static GPU_Image* ship1Texture;
	static GPU_Image* ship2Texture;

	static GPU_Image* planetTextures[7];
	static GPU_Image* backgroundTextures[3];
	static GPU_Image* solTexture;

	static GPU_Image* asterVoidTexture;


	static TTF_Font* font_30;
	static TTF_Font* font_30_outline;

	static Shader tintShader;
};