
#include "player.h"

#include "input.h"
#include "assets.h"
#include "animation.h"
#include "bullet.h"
#include "window.h"

const float distanceFromPlanet = 200.f;

const float accel = 0.5f;
const float maxVel = 1.f;
const float friction = 0.7f;

Player::Player(int id, vec planet_center) 
	: id(id)
	, planet_center(planet_center)
	, angle(90.f)
	, angularVel(0.f)
	, invertControls(false)
{
}

void Player::Update(float dt)
{
	if (Input::IsJustPressed(id, GameKeys::LEFT) || Input::IsJustPressed(id, GameKeys::RIGHT)) {
		invertControls = angle > 180;
	}
	
	if (Input::IsPressed(id, GameKeys::RIGHT)) {
		if (invertControls) {
			angularVel += accel;
		}
		else {
			angularVel -= accel;
		}
	}

	if (Input::IsPressed(id, GameKeys::LEFT)) {
		if (invertControls) {
			angularVel -= accel;
		}
		else {
			angularVel += accel;
		}
	}

	Mates::Clamp(angularVel, -maxVel, maxVel);
	
	Debug::out << angle << " " << angularVel << " " << invertControls;

	angle += angularVel;
	
	if (angle > 360.f) angle -= 360.f;
	else if (angle < 0.f) angle += 360.f;

	angularVel *= friction;

	pos = planet_center + vec::FromAngle(Mates::DegsToRads(angle)) * distanceFromPlanet;
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
