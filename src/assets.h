#pragma once

#include <array>

#include "sound.h"
#include "shader.h"

struct GPU_Image;
typedef struct _TTF_Font TTF_Font;
typedef struct _Mix_Music Mix_Music;

namespace Assets
{
	extern void LoadAll();

	inline GPU_Image* ship1Texture;
	inline GPU_Image* ship2Texture;

	inline GPU_Image* planetTextures[7];
	inline GPU_Image* backgroundTextures[3];
	inline GPU_Image* solTexture;

	inline GPU_Image* asterVoidTexture;

	inline TTF_Font* font_30;
	inline TTF_Font* font_30_outline;

	inline Sound bump;

	inline Sound hit;
	inline Sound loadshot;
	inline Sound shot_small[3];
	inline Sound shot_medium[2];
	inline Sound shot_large[1];
	inline Sound shield;
};