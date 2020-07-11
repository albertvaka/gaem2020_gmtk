#include "scene_main.h"
#include "input.h"
#ifdef _IMGUI
#include "imgui.h"
#endif
#include "assets.h"
#include "debug.h"
#include "collide.h"
#include "asteroid.h"
#include "sol.h"

SceneMain::SceneMain()
	: player1(0, 0)
	, player2(1, 1)
	, alienPartSys(Assets::invadersTexture)
{
}

void SceneMain::EnterScene()
{
	new Planet(0, 300, 50, 5000, 500, 8);
	new Planet(1, 370, 220, 5000, 500, 8);
	new Sol(vec(Window::GAME_WIDTH/2,Window::GAME_HEIGHT/2), 30);
}

void SceneMain::ExitScene()
{
	Planet::DeleteAll();
}

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

	for (Sol* sol : Sol::GetAll()) {
		sol->Update(dt);
	}

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


	for (const Planet* planet : Planet::GetAll()) {
		planet->Draw();
	}

	player1.Draw();
	player2.Draw();

	for (const Asteroid* a : Asteroid::GetAll()) {
		a->Draw();
		if (Debug::Draw) {
			a->DrawBounds(255,0,0);
		}
	}

	for (Sol* sol : Sol::GetAll()) {
		sol->Draw();
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
