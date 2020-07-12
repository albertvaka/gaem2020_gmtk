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


const float cicleTexturesTime = 2.f;

const float cameraOffsetY = 170.f;

extern float mainClock;

struct SceneIntro : public Scene {

	Planet p1, p2, p3;
	Sol sol;
	int currentLevel;
	float timer = 0.f;

	Text credits;
	Text startText;
	RotoText rototext;

	SceneIntro()
		: startText(Assets::font_30, Assets::font_30_outline), credits(Assets::font_30, Assets::font_30_outline)
		, p1(200, 0, 10000, 30)
		, p2(325, 250, 10000, 18)
		, p3(450, 180, 10000, 12)
		, sol(vec(Window::GAME_WIDTH / 2, Window::GAME_HEIGHT / 2))
	{
		p1.health = -1;
		p2.health = -1;
		p3.health = -1;
		RotateTextures();

		credits.setString("A game by:\n\n\nMarce Coll\n\nGerard del Castillo\n\nAlbert Vaca Cintora\n\n\nArt by:\n\n\nLVGames, Vectorpixelstar, Helianthus Games, Ravenmore");

		rototext.ShowMessage("El JAMON TURBO GAEM");
		rototext.Update(100);

		startText.setString("PRESS START");

	}

	void RotateTextures() {
		int tex;
		do {
			tex = Random::roll(std::size(Assets::backgroundTextures));
		} while (currentLevel == tex);
		currentLevel = tex;
		
		do {
			tex = Random::roll(std::size(Assets::planetTextures));
		} while (p1.planetTexture == tex);
		p1.planetTexture = tex;
		do {
			tex = Random::roll(std::size(Assets::planetTextures));
		} while (p1.planetTexture == tex || p2.planetTexture == tex);
		p2.planetTexture = tex;
		do {
			tex = Random::roll(std::size(Assets::planetTextures));
		} while (p1.planetTexture == tex || p2.planetTexture == tex || p3.planetTexture == tex);
		p3.planetTexture = tex;
	}

	void EnterScene() override {
		Camera::SetCenter(Camera::GetCenter() + vec(0, -cameraOffsetY));
	}
	void ExitScene() override { 
		Camera::SetCenter(Camera::GetCenter() + vec(0, cameraOffsetY));
	}

	void Update(float dt) override
	{
		if (Input::IsPressedAnyPlayer(GameKeys::START)) {
			SceneManager::SetScene(new SceneMain());
			return;
		}

		rototext.Update(dt);

		for (Sol* sol : Sol::GetAll()) {
			sol->Update(dt);
		}

		for (Planet* planet : Planet::GetAll()) {
			planet->Update(dt);
		}

		timer += dt;
		if (timer >= cicleTexturesTime) {
			timer -= cicleTexturesTime;
			RotateTextures();
		}
	}

	void Draw()
	{
		Window::Clear(0, 0, 0);
		GPU_Image* bgAsset = Assets::backgroundTextures[currentLevel];
		Window::Draw(bgAsset, vec(0, -cameraOffsetY))
			.withScale(4)
			.withRect(
				0, 0,
				Window::GAME_WIDTH, Window::GAME_HEIGHT
			);

		for (const Planet* planet : Planet::GetAll()) {
			planet->Draw();
		}

		for (Sol* sol : Sol::GetAll()) {
			sol->Draw();
		}

		rototext.Draw(Camera::GetCenter() - vec(0, 190));

		if ((int(mainClock * 1000) / 350) % 2) {
			Window::Draw(startText, Camera::GetCenter() + vec(0, cameraOffsetY))
				.withOrigin(startText.getSize()/2);
		}

		Window::Draw(credits, vec(30, 1000-cameraOffsetY-30-credits.getSize().y))
			.withScale(0.9f,1.f);
	}

};

