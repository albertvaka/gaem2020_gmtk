#include "scene_main.h"
#include "input.h"
#ifdef _IMGUI
#include "imgui.h"
#endif
#include "bullet.h"
#include "assets.h"
#include "debug.h"
#include "collide.h"
#include "alien.h"

SceneMain::SceneMain() :
	planet_one(vec(Window::GAME_WIDTH/4, Window::GAME_HEIGHT/2), 100),
	planet_two(vec(3*Window::GAME_WIDTH / 4, Window::GAME_HEIGHT / 2), 100)
{
}

void SceneMain::EnterScene() 
{
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

}

void SceneMain::Draw()
{
	Window::Clear(0, 0, 0);
	
	planet_one.Draw();
	planet_two.Draw();

	//alienPartSys.DrawImGUI();
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
