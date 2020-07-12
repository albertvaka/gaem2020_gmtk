#include "input.h"

#include "magic_enum.h"
#include "../src/ia.h"
#include <functional>

std::function<bool()> kb_map[magic_enum::enum_count<GameKeys>()];
std::function<bool(int)> gp_map[magic_enum::enum_count<GameKeys>()];
std::function<bool(int)> ia_map[magic_enum::enum_count<GameKeys>()];

auto ias = Ia::GetAll();

inline int getIaIndex(int p) {
	for (int i = 0; i < ias.size(); i++) {
		if (ias[i]->id == p)
			return i;
	}
	return -1;
}

inline void RemapIaInput()
{
	ia_map[GameKeys::UP] = [](int p) {
		int i = getIaIndex(p);
		return ias[i]->isGameKeyPressed(GameKeys::UP);
	};
	ia_map[GameKeys::DOWN] = [](int p) {
		int i = getIaIndex(p);
		return ias[i]->isGameKeyPressed(GameKeys::DOWN);
	};
	ia_map[GameKeys::LEFT] = [](int p) {
		int i = getIaIndex(p);
		return ias[i]->isGameKeyPressed(GameKeys::LEFT);
	};
	ia_map[GameKeys::RIGHT] = [](int p) {
		int i = getIaIndex(p);
		return ias[i]->isGameKeyPressed(GameKeys::RIGHT);
	};

	ia_map[GameKeys::CANNON_UP] = [](int p) {
		return false;
	};
	ia_map[GameKeys::CANNON_DOWN] = [](int p) {
		return false;
	};
	ia_map[GameKeys::CANNON_LEFT] = [](int p) {
		return false;
	};
	ia_map[GameKeys::CANNON_RIGHT] = [](int p) {
		return false;
	};
	ia_map[GameKeys::SHOOT] = [](int p) { 
		int i = getIaIndex(p);
		return ias[i]->isGameKeyPressed(GameKeys::SHOOT); 
	};
	ia_map[GameKeys::SHIELD] = [](int p) { 
		int i = getIaIndex(p);
		return ias[i]->isGameKeyPressed(GameKeys::SHIELD);
	};
	ia_map[GameKeys::START] = [](int p) { return false; };
}

inline void RemapGamePadInput()
{
	gp_map[GameKeys::UP] = [](int p) {
		return 	GamePad::IsButtonPressed(p, SDL_CONTROLLER_BUTTON_DPAD_UP);
	};
	gp_map[GameKeys::DOWN] = [](int p) {
		return GamePad::IsButtonPressed(p, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	};
	gp_map[GameKeys::LEFT] = [](int p) {
		return GamePad::IsButtonPressed(p, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	};
	gp_map[GameKeys::RIGHT] = [](int p) {
		return GamePad::IsButtonPressed(p, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	};

	gp_map[GameKeys::CANNON_UP] = [](int p) {
		return GamePad::AnalogStick::Right.get(p, 50.f).y < -50.0f ||
			GamePad::IsButtonPressed(p, SDL_CONTROLLER_BUTTON_DPAD_UP);
	};
	gp_map[GameKeys::CANNON_DOWN] = [](int p) {
		return GamePad::AnalogStick::Right.get(p, 50.f).y > 50.0f ||
			GamePad::IsButtonPressed(p, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	};
	gp_map[GameKeys::CANNON_LEFT] = [](int p) {
		return GamePad::AnalogStick::Right.get(p, 50.f).x < 0.0f ||
			GamePad::IsButtonPressed(p, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	};
	gp_map[GameKeys::CANNON_RIGHT] = [](int p) {
		return GamePad::AnalogStick::Right.get(p, 50.f).x > 0.0f ||
			GamePad::IsButtonPressed(p, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	};
	gp_map[GameKeys::SHOOT] = [](int p) { return GamePad::Trigger::Right.get(p) > 30.f; };
	gp_map[GameKeys::SHIELD] = [](int p) { return GamePad::IsButtonPressed(p, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER); };
	gp_map[GameKeys::START] = [](int p) { return GamePad::IsButtonPressed(p, SDL_CONTROLLER_BUTTON_START) || GamePad::IsButtonPressed(p, SDL_CONTROLLER_BUTTON_A); };
}

inline void RemapKeyboardInput()
{
	kb_map[GameKeys::UP] = []() {
		return Keyboard::IsKeyPressed(SDL_SCANCODE_W) || Keyboard::IsKeyPressed(SDL_SCANCODE_UP);
	};
	kb_map[GameKeys::DOWN] = []() {
		return Keyboard::IsKeyPressed(SDL_SCANCODE_S) || Keyboard::IsKeyPressed(SDL_SCANCODE_DOWN);
	};
	kb_map[GameKeys::LEFT] = []() {
		return Keyboard::IsKeyPressed(SDL_SCANCODE_A) || Keyboard::IsKeyPressed(SDL_SCANCODE_LEFT);
	};
	kb_map[GameKeys::RIGHT] = []() {
		return Keyboard::IsKeyPressed(SDL_SCANCODE_D) || Keyboard::IsKeyPressed(SDL_SCANCODE_RIGHT);
	};

	kb_map[GameKeys::CANNON_RIGHT] = []() {
		return Keyboard::IsKeyPressed(SDL_SCANCODE_E);
	};
	kb_map[GameKeys::CANNON_LEFT] = []() {
		return Keyboard::IsKeyPressed(SDL_SCANCODE_Q);
	};
	kb_map[GameKeys::SHOOT] = []() {
		return Keyboard::IsKeyPressed(SDL_SCANCODE_SPACE);
	};
	kb_map[GameKeys::START] = []() {
		return Keyboard::IsKeyPressed(SDL_SCANCODE_RETURN) || Keyboard::IsKeyPressed(SDL_SCANCODE_ESCAPE);
	};
}



//int player_to_joystick[Input::kMaxPlayers] = { nullptr };

// Assumes first player is on Keyboard.
int Input::keyboard_player_id = 1;
KeyStates Input::action_states[Input::kMaxPlayers][magic_enum::enum_count<GameKeys>()] = { { RELEASED } };
float Input::action_times[Input::kMaxPlayers][magic_enum::enum_count<GameKeys>()] = { { 0 } };


void Input::Update(float dt)
{
	ias = Ia::GetAll();
	for (int player = 0; player < Input::kMaxPlayers; ++player) {
		int gamepad_id = player;
		const int iaIndex = getIaIndex(player);
		bool isIa = iaIndex > -1;
		for (size_t k = 1; k < magic_enum::enum_count<GameKeys>(); k++) {  //Skip GameKeys::NONE
			bool pressed_now = (gp_map[k] && gp_map[k](gamepad_id));
			if (player == keyboard_player_id) {
				pressed_now = pressed_now || (kb_map[k] && kb_map[k]());
			}
			if (isIa) {
				//pressed_now = pressed_now || (ia_map[k] && ia_map[k](player));
				pressed_now = (ia_map[k] && ia_map[k](player));
			}
			if (pressed_now) {
				if (action_states[player][k] == JUST_PRESSED || action_states[player][k] == PRESSED) {
					action_states[player][k] = PRESSED;
					if (action_times[player][k] < 1000.f) action_times[player][k] += dt;
				}
				else {
					action_states[player][k] = JUST_PRESSED;
					action_times[player][k] = dt;
				}
			}
			else {
				if (action_states[player][k] == JUST_RELEASED || action_states[player][k] == RELEASED) {
					action_states[player][k] = RELEASED;
					if (action_times[player][k] < 1000.f) action_times[player][k] += dt;
				}
				else {
					action_states[player][k] = JUST_RELEASED;
					action_times[player][k] = dt;
				}
			}
		}
	}
}


void Input::Init()
{
	RemapGamePadInput();
	RemapKeyboardInput();
	RemapIaInput();
}

