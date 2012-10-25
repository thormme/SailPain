#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include <zenilib.h>
#include "Level.h"
#include "GameObject.h"
#include "Viewport.h"
#include "Player.h"

class PlayState : public Zeni::Gamestate_Base {
  PlayState(const PlayState &);
  PlayState operator=(const PlayState &);

public:
  PlayState(std::vector<Player*> players, std::string level);
  ~PlayState();

private:
	Level * m_level;
	std::vector<Viewport*> m_viewports;
	std::vector<Player*> m_players;
	std::vector<GameObject*> m_gameObjects;
	Zeni::Chronometer<Zeni::Time> m_chronometer;
	float m_timePassed;

	void on_push();

	void on_pop();

	void PlayState::on_key(const SDL_KeyboardEvent &event);

	void PlayState::on_joy_button(const SDL_JoyButtonEvent &event);

	void on_joy_axis(const SDL_JoyAxisEvent &event);

	void perform_logic();

	void render();


	const std::vector<std::vector<GameObject*>> getGameObjectCollisions(); ///< Get collisions between bodies, returns vector for each Body

	void addGameObject(GameObject * object); ///< Add an object to the world.

	void removeGameObject(GameObject * object); ///< Remove an object from the world.

	void applyStateModifications(StateModifications &stateModifications);
};
#endif