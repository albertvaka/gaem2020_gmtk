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
#include "rand.h"
#include "scene_manager.h"

SceneMain::SceneMain()
	: player1(0, 0)
	, player2(1, 1)
	, alienPartSys(Assets::asterVoidTexture)
{
	alienPartSys.AddSprite(AnimLib::ASTERVOID[1].rect);
	alienPartSys.AddSprite(AnimLib::ASTERVOID[7].rect);

	float vel = 15;
	alienPartSys.max_vel = vec(vel, vel);
	alienPartSys.min_vel = vec(-vel, -vel);
	alienPartSys.min_ttl = 1.5f;
	alienPartSys.max_ttl = 2.0f;
	alienPartSys.min_interval = 0.03f;
	alienPartSys.max_interval = 0.06f;
	alienPartSys.scale_vel = -0.3f;
	alienPartSys.min_rotation = 0.f;
	alienPartSys.max_rotation = 360.f;
	alienPartSys.rotation_vel = 180.f;
	alienPartSys.alpha = 0.75f;
}

void SceneMain::EnterScene()
{
	Camera::SetZoom(10);
	player1.planet = new Planet(350, 0, 5000, 8);
	player2.planet = new Planet(350, 180, 5000, 8);
	new Sol(vec(Window::GAME_WIDTH/2,Window::GAME_HEIGHT/2), 30);
}

void SceneMain::ExitScene()
{
	alienPartSys.Clear();
	Planet::DeleteAll();
	Asteroid::DeleteAll();
	Sol::DeleteAll();
}

void SceneMain::Update(float dt)
{
	float zoom = Camera::GetZoom();
	if (zoom > 1) {
		Camera::SetZoom(Mates::MaxOf(1.f, zoom - 15 * dt));
		return;
	}

#ifdef _DEBUG
	const SDL_Scancode restart = SDL_SCANCODE_ESCAPE;
	if (Keyboard::IsKeyJustPressed(restart)) {
		SceneManager::SetScene(new SceneMain());
		return;
	}

	if (rototext.shown) {
		rototext.Update(dt);
		if (rototext.timer > 2.f && Input::IsPressedAnyPlayer(GameKeys::SHOOT)) {
			SceneManager::SetScene(new SceneMain());
			return;
		}
		return;
	}

	if (player1.planet->health <= 0 && player2.planet->health <= 0) {
		rototext.ShowMessage("WTF?");
	}
	else if (player1.planet->health <= 0) {
		rototext.ShowMessage("Player 2 wins!");
	}
	else if (player2.planet->health <= 0) {
		rototext.ShowMessage("Player 1 wins!");
	}

#ifdef _DEBUG
	if (Mouse::IsJustPressed()) {
		new Asteroid(Random::rollf(0.2, 3), Mouse::GetPositionInWindow(), Random::vecInRange(vec(-100,-100),vec(100,100)));
	}
#endif

	if (Keyboard::IsKeyJustPressed(SDL_SCANCODE_F4)) {
		rototext.ShowMessage("P1 wins");
	}
#endif

	for (Sol* sol : Sol::GetAll()) {
		sol->Update(dt);
	}

	if (Keyboard::IsKeyJustPressed(SDL_SCANCODE_F7)) {
		currentLevel = (currentLevel + 1) % std::size(Assets::backgroundTextures);
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
		alienPartSys.pos = a->pos;
		alienPartSys.min_scale = 0.1f * sqrt(a->size);
		alienPartSys.max_scale = 0.2f * sqrt(a->size);
		alienPartSys.Spawn(dt);
	}

#ifdef _IMGUI
	ImGui::End();
#endif

#ifdef _IMGUI
	{
		ImGui::Begin("scene");
		ImGui::SliderFloat("P1 health", &(player1.planet->health), 0.f, 100.f);
		ImGui::SliderFloat("P2 health", &(player2.planet->health), 0.f, 100.f);
		ImGui::End();
	}
#endif

	alienPartSys.UpdateParticles(dt);
	Asteroid::DeleteNotAlive();
}

void SceneMain::Draw()
{
	Window::Clear(0, 0, 0);
	GPU_Image* bgAsset = Assets::backgroundTextures[currentLevel];
	Window::Draw(bgAsset, vec(0, 0))
		.withScale(3)
		.withRect(
			0, 0,
			Window::GAME_WIDTH, Window::GAME_HEIGHT
		);

	for (const Planet* planet : Planet::GetAll()) {
		planet->Draw();
	}

	alienPartSys.Draw();

	for (const Asteroid* a : Asteroid::GetAll()) {
		a->Draw();
		if (Debug::Draw) {
			a->DrawBounds(255,0,0);
		}
	}

	player1.Draw();
	player2.Draw();

	for (Sol* sol : Sol::GetAll()) {
		sol->Draw();
	}


	rototext.Draw();

}
