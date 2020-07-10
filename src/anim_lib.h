#pragma once

#include "SDL_gpu.h"

struct AnimationFrame
{
	const GPU_Rect rect;
	const float duration;
};

namespace AnimLib {

	constexpr const AnimationFrame ALIEN_1[] = {
		{ {0,0,71,55}, 0.3f },
		{ {71,0,71,55}, 0.3f },
	};

	constexpr const AnimationFrame ALIEN_2[] = {
		{ {144,0,58,55}, 0.3f },
		{ {144 + 58,0,58,55}, 0.3f },
	};

	constexpr const GPU_Rect PLAYER = { 73,317,44,70 };
	constexpr const GPU_Rect CANNON = { 241,196,20,50 };
	constexpr const GPU_Rect ASTEROID = { 0, 0, 71, 55 };
	

}
