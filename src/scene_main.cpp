#include "scene_main.h"
#include "input.h"
#ifdef _IMGUI
#include "imgui.h"
#endif
#include "bullet.h"
#include "assets.h"
#include "debug.h"
#include "collide.h"
#include "asteroid.h"

SceneMain::SceneMain()
	: planet_one(vec(Window::GAME_WIDTH/4, Window::GAME_HEIGHT/2), 100)
	, planet_two(vec(3*Window::GAME_WIDTH / 4, Window::GAME_HEIGHT / 2), 100)
	, player1(0, planet_one.pos)
	, player2(1, planet_two.pos)
	, alienPartSys(Assets::invadersTexture)

{
}

void SceneMain::EnterScene()
{
	Debug::out << "Create asteroid";
	new Asteroid(100, vec(10, 10), vec(10, 10));
	new Asteroid(100, vec(200, 200), vec(10, 10));
	new Asteroid(100, vec(30, 50), vec(10, 10));
	Debug::out << "End asteroid";
}

void SceneMain::ExitScene()
{
}

void SceneMain::Update(float dt)
{
	planet_one.Update(dt);
	planet_two.Update(dt);
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

	Bullet::DeleteNotAlive();
	Asteroid::DeleteNotAlive();
}

void SceneMain::Draw()
{
	Window::Clear(0, 0, 0);

	planet_one.Draw();
	planet_two.Draw();

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
