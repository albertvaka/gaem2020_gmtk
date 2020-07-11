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
	: planets({Planet(vec(Window::GAME_WIDTH/4, Window::GAME_HEIGHT/2), 100),
			   Planet(vec(3 * Window::GAME_WIDTH/4, Window::GAME_HEIGHT/2), 100)})
	, player1(0, planets[0].pos)
	, player2(1, planets[1].pos)
	, alienPartSys(Assets::invadersTexture)
{
}

void SceneMain::EnterScene()
{
	planets.push_back(Planet(vec(Window::GAME_WIDTH/2, Window::GAME_HEIGHT/2), 100));
//	planets.push_back(Planet(vec(Window::GAME_WIDTH/2, Window::GAME_HEIGHT/3), 100));
//	planets.push_back(Planet(vec(Window::GAME_WIDTH/2, 2 * Window::GAME_HEIGHT/3), 100));
}

void SceneMain::ExitScene()
{}

void SceneMain::Update(float dt)
{
	for (auto planet : planets) {
		planet.Update(dt);
	}

	player1.Update(dt);
	player2.Update(dt);

#ifdef _DEBUG
	const SDL_Scancode restart = SDL_SCANCODE_F5;
	if (Keyboard::IsKeyJustPressed(restart)) {
		ExitScene();
		EnterScene();
		return;
	}
#endif

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
