#include <zenilib.h>
#include "Level.h"

class PlayState : public Zeni::Gamestate_Base {
  PlayState(const PlayState &);
  PlayState operator=(const PlayState &);

public:
  PlayState();

private:
	Zeni::Camera m_camera;
	Level m_level;

	void on_push();

	void on_pop();

	void PlayState::on_key(const SDL_KeyboardEvent &event);

	void PlayState::on_joy_button(const SDL_JoyButtonEvent &event);

	void on_joy_axis(const SDL_JoyAxisEvent &event);

	void perform_logic();

	void render();
};