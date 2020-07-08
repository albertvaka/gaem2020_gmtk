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

SceneMain::SceneMain()
	: player(0, 200)
	, alienPartSys(Assets::invadersTexture)
{
	alienPartSys.AddSprite(AnimLib::ALIEN_1[0].rect);
	alienPartSys.AddSprite(AnimLib::ALIEN_2[0].rect);
	alienPartSys.min_scale = 0.15f;
	alienPartSys.max_scale = 0.4f;
	alienPartSys.max_vel = vec(50,50);
	alienPartSys.min_vel = vec(-50,-50);
	alienPartSys.min_rotation = 0;
	alienPartSys.max_rotation = 360;
	alienPartSys.rotation_vel = 100.f;
	alienPartSys.scale_vel = -0.2f;
}

void SceneMain::EnterScene() 
{
	SpawnAliens();
}

void SceneMain::SpawnAliens() {
	for (int angle = 0; angle < 360; angle += 360/currentLevel) {
		new Alien(angle, Random::rollf(250,350));
	}
}

void SceneMain::ExitScene()
{
	alienPartSys.Clear();
	Bullet::DeleteAll();
	Alien::DeleteAll();
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

	player.Update(dt);

	for (Alien* a : Alien::GetAll()) {
		a->Update(dt);
	}

	for (Bullet* b : Bullet::GetAll()) {
		b->Update(dt);
		for (Alien* a  : Alien::GetAll()) {
			if (Collide(a,b)) {
				Debug::out << "KABOOM " << a->pos;

				alienPartSys.pos = a->pos;
				alienPartSys.AddParticles(10);
				
				a->alive = false;
				b->alive = false;
			}
		}
	}

	if (Alien::GetAll().empty()) {
		currentLevel++;
		SpawnAliens();
	}

	Bullet::DeleteNotAlive();
	Alien::DeleteNotAlive();

	alienPartSys.UpdateParticles(dt);

}

void SceneMain::Draw()
{
	Window::Clear(0, 0, 0);

	Window::Draw(Assets::backgroundTexture, Camera::GetCenter())
		.withOrigin(Assets::backgroundTexture->w/2, Assets::backgroundTexture->h/2);

	player.Draw();

	for (const Alien* a : Alien::GetAll()) {
		a->Draw();
		if (Debug::Draw) {
			a->DrawBounds(255,0,0);
		}
	}

	for (const Bullet* b : Bullet::GetAll()) {
		b->Draw();
		if (Debug::Draw) {
			b->DrawBounds(255,0,0);
		}
	}

	alienPartSys.Draw();

	//alienPartSys.DrawImGUI();

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

}
