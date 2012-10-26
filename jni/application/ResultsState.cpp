#include <zenilib.h>
#include "ResultsState.h"
#include <sstream>

void ResultsState::render() {
	Widget_Gamestate::render();

	Zeni::Font &fr = Zeni::get_Fonts()["system_36_800x600"];


	std::ostringstream str;
	if (m_winnerIndex.size() == 1) {
		str << "Player " << m_winnerIndex[0] + 1 << " is the winner!";
	} else {
		str << "Players " << m_winnerIndex[0] + 1;
		for (int i=1; i < m_winnerIndex.size(); i++) {
			str << " and " << m_winnerIndex[i] + 1;
		}
		str << " tied for first!";
	}
	

	fr.render_text(Zeni::String(str.str()),
					Zeni::Point2f(400.0f, 150.0f - 0.5f * fr.get_text_height()),
					Zeni::get_Colors()["title_text"],
					Zeni::ZENI_CENTER);

	Zeni::Vector2f carImageSize(100.0f, 50.0f);
	/*for (int i=0; i < m_finishOrderedPlayers.size(); i++) {
		Zeni::Point2f carImagePosition(800.0f/(m_finishOrderedPlayers.size())*m_finishOrderedPlayers[i] + 50.0f, 450.0f);

		std::ostringstream place;
		place << i + 1;

		fr.render_text(Zeni::String(place.str()),
			carImagePosition + Zeni::Vector2f(50.0f, -40.0f),
						Zeni::get_Colors()["title_text"],
						Zeni::ZENI_CENTER);

		//Zeni::render_image(m_players[m_finishOrderedPlayers[i]]->getCarImage(), carImagePosition, carImagePosition + carImageSize);
	}*/

	/*if (m_finishPlace >= 0 && m_finishPlace <= 3) {
		Zeni::String image = "placeholder";
		if (m_finishPlace == 0) {
			image = "trophy-gold";
		} else if (m_finishPlace == 1) {
			image = "trophy-silver";
		} else if (m_finishPlace == 2) {
			image = "trophy-bronze";
		}
		Zeni::render_image(image, Zeni::Point2f(350.0f, 250.0f), Zeni::Point2f(450.0f, 350.0f));
	}*/
}

ResultsState::ResultsState(std::vector<Player*> players, std::vector<Sailplane::Stats> stats)
	: Widget_Gamestate(std::make_pair(Zeni::Point2f(0.0f, 0.0f), Zeni::Point2f(800.0f, 600.0f))),
	m_players(players), m_stats(stats) {
		
	int bestScore = -1000;
	for (int i=0; i < m_stats.size(); i++) {
		int score = m_stats[i].kills - m_stats[i].deaths;
		if (score > bestScore) {
			bestScore = score;
			m_winnerIndex.empty();
			m_winnerIndex.push_back(i);
		} else if (score == bestScore) {
			m_winnerIndex.push_back(i);
		}
	}
	//UserData::setBestPosition(mapName.std_str(), m_finishPlace);
}