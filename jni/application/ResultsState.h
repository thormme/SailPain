#ifndef RESULTSSTATE_H
#define RESULTSSTATE_H
#include <zenilib.h>
#include "Player.h"

class ResultsState : public Zeni::Widget_Gamestate {
	ResultsState(const ResultsState &);
	ResultsState operator=(const ResultsState &);

	std::vector<Player*> m_players;
	std::vector<Sailplane::Stats> m_stats;
	std::vector<int> m_winnerIndex;

	int m_finishPlace;

public:
	ResultsState(std::vector<Player*> players, std::vector<Sailplane::Stats> stats);

private:

	void on_key(const SDL_KeyboardEvent &event) {
		if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED) {
			Zeni::get_Game().pop_state();
		}
	}

	void render();
};
#endif