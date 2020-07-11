#include "scene_main.h"
#include "input.h"
#ifdef _IMGUI
#include "imgui.h"
#endif
#include "assets.h"
#include "debug.h"
#include "collide.h"
#include "asteroid.h"

SceneMain::SceneMain()
	: planets()
	, player1(0, 0)
	, player2(1, 1)
	, alienPartSys(Assets::invadersTexture)
{
}

void SceneMain::EnterScene()
{
	planets.push_back(Planet(300, 50, 1000000, 500, 8));
	planets.push_back(Planet(370, 220, 1000000, 500, 8));
	planets.push_back(Planet(0, 0, 2000000, 500, 0));
	planets.push_back(Planet(120, 0, 500000, 500, -6));
}

void SceneMain::ExitScene()
{}

void SceneMain::Update(float dt)
{
#ifdef _DEBUG
	const SDL_Scancode restart = SDL_SCANCODE_F5;
	if (Keyboard::IsKeyJustPressed(restart)) {
		ExitScene();
		EnterScene();
		return;
	}
#endif

	for (Planet* planet : Planet::GetAll()) {
		planet->Update(dt);
	}

	player1.Update(dt);
	player2.Update(dt);

#ifdef _IMGUI
	ImGui::Begin("Asteroids");
#endif

	for (Asteroid* a : Asteroid::GetAll()) {
		a->Update(dt);
	}

#ifdef _IMGUI
	ImGui::End();
#endif

	Asteroid::DeleteNotAlive();
}

void SceneMain::Draw()
{
	Window::Clear(0, 0, 0);

	for (auto planet : planets) {
		planet.Draw();
	}

	player1.Draw();
	player2.Draw();

	for (const Asteroid* a : Asteroid::GetAll()) {
		a->Draw();
		if (Debug::Draw) {
			a->DrawBounds(255,0,0);
		}
	}

/*
#ifdef _IMGUI
	{
		ImGui::Begin("scene");
		vec m = Mouse::GetPositionInWorld();
		ImGui::Text("Mouse: %f,%f", m.x, m.y);
		if (ImGui::SliderInt("level", &currentLevel, 1, 20)) {
			Alien::DeleteAll();
			SpawnAliens();
		};
		ImGui::End();
	}
#endif
*/
}
