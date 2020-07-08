#pragma once

#include <array>

#include "vec.h"
#include "bounds.h"
#include "SDL_gpu.h"

struct TiledTiles
{
	static const GPU_Rect tileToTextureRect[];

	enum Value : uint8_t
	{
		NONE = 0,
		BG_1,
		BG_2,
		BG_3,
		BG_4,
		BG_5,
		BG_6,
		BG_7,
		BG_8,
		BG_9,
		BG_10,
		BG_11,
		BG_12,
		BG_13,
		ONEWAY_1,
		RSLOPE_1,
		LSLOPE_1,
		SOLID_1,
		BREAKABLE_1,
		SOLID_TRANSPARENT,
	};

	static const Value ONEWAY_BEGIN = ONEWAY_1;
	static const Value RSLOPE_BEGIN = RSLOPE_1;
	static const Value LSLOPE_BEGIN = LSLOPE_1;
	static const Value SOLID_BEGIN = SOLID_1;
	static const Value BREAKABLE_BEGING = BREAKABLE_1;

	// Aliases

};

struct TiledMap
{
	static const uint8_t map[];
	static const vec map_size;
	static const std::array<Bounds, 0> screens;
};

struct TiledEntities
{
	
};

struct TiledAreas
{
	
};