
#include "player.h"

#include "input.h"
#include "assets.h"
#include "animation.h"
#include "bullet.h"
#include "window.h"


const float bulletSpeed = 200.f;

Player::Player(float angle, float distance)
	: distance(distance)
{
}

void Player::Update(float dt)
{
	vec mouseFromCenter = Mouse::GetPositionInWorld() - Camera::GetCenter();
	pos = Camera::GetCenter() + mouseFromCenter.Normalized() * distance;
	
	if (Mouse::IsJustPressed(Mouse::Button::Left)) {
		new Bullet(pos, mouseFromCenter.Normalized() * bulletSpeed);
	}

	if (Input::IsPressed(0, UP)) {
		this->distance += 100 * dt;
	}

	if (Input::IsPressed(0, DOWN)) {
		this->distance -= 100 * dt;
	}
}

void Player::Draw() const
{
	const GPU_Rect& animRect = AnimLib::PLAYER;
	Window::Draw(Assets::invadersTexture, pos)
		.withRect(animRect)
		.withColor({0,255,255,255})
		.withOrigin(vec(animRect.w, 0)/2)
		.withRotation(Camera::GetCenter().Angle(pos) + 90);
}
