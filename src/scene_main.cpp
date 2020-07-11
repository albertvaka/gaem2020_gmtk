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
	: player1(0)
	, player2(1)
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
	//alienPartSys.min_rotation = 0.f;
	//alienPartSys.max_rotation = 360.f;
	//alienPartSys.rotation_vel = 180.f;
	//alienPartSys.alpha = 0.75f;
	currentLevel = Random::roll(std::size(Assets::backgroundTextures));
}

void SceneMain::EnterScene()
{
	Camera::SetZoom(10);
	player1.planet = new Planet(350, 0, 10000, 8);
	player2.planet = new Planet(350, 180, 10000, 8);
	new Sol(vec(Window::GAME_WIDTH/2,Window::GAME_HEIGHT/2));
}

void SceneMain::ExitScene()
{
	alienPartSys.Clear();
	Planet::DeleteAll();
	Asteroid::DeleteAll();
	Sol::DeleteAll();
}

void AsteroidCollision(Asteroid* a, Asteroid* b) {
	float diff_pos = (b->pos - a->pos).Length();

	vec a_vel = a->velocity
	- (2 * b->mass)/(b->mass + a->mass)
	* ((a->velocity - b->velocity).Dot(a->pos - b->pos) / (diff_pos * diff_pos))
	* (a->pos - b->pos);

	vec b_vel = b->velocity
	- (2 * a->mass)/(b->mass + a->mass)
	* ((b->velocity - a->velocity).Dot(b->pos - a->pos) / (diff_pos * diff_pos))
	* (b->pos - a->pos);

	a->velocity = a_vel;
	b->velocity = b_vel;
	a->max_speed_mult = 2.f;
	b->max_speed_mult = 2.f;

	vec a_to_b = (b->pos - a->pos).Normalized();
	vec b_to_a = (a->pos - b->pos).Normalized();
	float bounds_diff = b->bounds().Distance(a->bounds());

	a->pos += a_to_b * bounds_diff / 2.0f;
	b->pos += b_to_a * bounds_diff / 2.0f;
}

void SceneMain::Update(float dt)
{
	float zoom = Camera::GetZoom();
	if (zoom > 1) {
		Camera::SetZoom(Mates::MaxOf(1.f, zoom - 15 * dt));
		return;
	}

	const SDL_Scancode restart = SDL_SCANCODE_ESCAPE;
	if (Keyboard::IsKeyJustPressed(restart)) {
		SceneManager::SetScene(new SceneMain());
		return;
	}

#ifdef _DEBUG
	if (Mouse::IsJustPressed()) {
		new Asteroid(Random::rollf(0.2, 3), Mouse::GetPositionInWindow(), Random::vecInRange(vec(-100, -100), vec(100, 100)));
	}
	if (Keyboard::IsKeyJustPressed(SDL_SCANCODE_F4)) {
		rototext.ShowMessage("P1 wins");
	}
#endif

	if (rototext.shown) {
		rototext.Update(dt);
		if (rototext.timer > 2.f && Input::IsPressedAnyPlayer(GameKeys::START)) {
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

	CollideSelf(Asteroid::GetAll(), &AsteroidCollision);

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
		const GPU_Rect& animRect = a->anim.GetCurrentRect();
		// Sorry no se que collons estic fent
		alienPartSys.pos = a->pos
			// Resta la meitat del sprite
			-((vec(animRect.w, animRect.h) / 2) 
			//Multiplicat per l¡escalat que es fa a withScale (entre 2 because why not)
			* (20 * sqrt(a->size)) / (animRect.w / 4))/2;
		alienPartSys.min_scale = 0.05f * sqrt(a->size);
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

#ifdef _IMGUI
	{
		alienPartSys.DrawImGUI("particles");
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
		.withScale(4)
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
		if (Debug::Draw) {
			sol->DrawBounds(255, 255, 0);
		}
	}

	rototext.Draw();
}
