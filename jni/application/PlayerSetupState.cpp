#include <zenilib.h>
#include "PlayerSetupState.h"
#include "Player.h"
#include "PlayState.h"

namespace {
	std::vector<Player*> players;
	std::string level;
	Zeni::String diagramImage;
}

PlayerSetupState::PlayerSetupState() 
		: m_mapSelect(Zeni::Point2f(500.0f, 225.0f), Zeni::Point2f(700.0f,275.0f), Zeni::Point2f(500.0f, 225.0f), Zeni::Point2f(700.0f,525), "system_36_800x600"),
		Widget_Gamestate(std::make_pair(Zeni::Point2f(0.0f, 0.0f), Zeni::Point2f(800.0f, 600.0f))) {
	m_widgets.lend_Widget(play_button);
	m_widgets.lend_Widget(m_mapSelect);
	for (int i=0; i < 4; i++) {
		players.push_back(new Player());
		m_controls.push_back(new ControlTypeSelect(players.back(), Zeni::Point2f(200.0f*(float)(i), 0.0f), Zeni::Point2f(200.0f*(float)(i+1),50.0f), Zeni::Point2f(200.0f*(float)(i), 0.0f), Zeni::Point2f(200.0f*(float)(i+1),350), "system_36_800x600"));
		if (i > 0) {
			m_controls[i]->select_option("Empty");
		}
		m_controls[i]->on_accept(m_controls[i]->get_selected());
		m_widgets.lend_Widget(*m_controls[i]);
	}
	m_controls[0]->on_accept(m_controls[0]->get_selected());
}

PlayerSetupState::~PlayerSetupState() {
	for (int i=0; i < m_controls.size(); i++) {
		delete m_controls[i];
	}
	m_controls.clear();
	for (int i=0; i < players.size(); i++) {
		delete players[i];
	}
	players.clear();
}

void PlayerSetupState::on_uncover() {
	m_mapSelect.on_accept(m_mapSelect.get_selected());
}

PlayerSetupState::ControlTypeSelect::ControlTypeSelect(Player * player, const Zeni::Point2f &upper_left_, const Zeni::Point2f &lower_right_, const Zeni::Point2f &expanded_upper_left_, const Zeni::Point2f &expanded_lower_right_, const Zeni::String &font_)
		: m_player(player), Selector(upper_left_, lower_right_, expanded_upper_left_, expanded_lower_right_, font_) {
			
	add_option("Keyboard");
	add_option("Gamepad 1");
	add_option("Gamepad 2");
	add_option("Gamepad 3");
	add_option("Gamepad 4");
	add_option("Empty");

	select_option("Keyboard");
}

void PlayerSetupState::ControlTypeSelect::on_accept(const Zeni::String &option) {
	diagramImage = "";
	if (m_player == nullptr) {
		m_player = new Player();
		players.push_back(m_player);
	}
	if (option == "Keyboard") {
		m_player->setControls();
		diagramImage = "keyboard_diagram";
	} else if (option == "Gamepad 1") {
		m_player->setControls(0);
		diagramImage = "controller_diagram";
	} else if (option == "Gamepad 2") {
		m_player->setControls(1);
		diagramImage = "controller_diagram";
	} else if (option == "Gamepad 3") {
		m_player->setControls(2);
		diagramImage = "controller_diagram";
	} else if (option == "Gamepad 4") {
		m_player->setControls(3);
		diagramImage = "controller_diagram";
	} else if (option == "Empty") {
		players.erase(std::find(players.begin(), players.end(), m_player));
		delete m_player;
		m_player = nullptr;
	}

	if (m_player != nullptr) {
		switch((int)(std::find(players.begin(), players.end(), m_player) - players.begin()) % 4) {
		case 0:
			m_player->setPlaneModel("models/plane.3ds");
			break;
		case 1:
			m_player->setPlaneModel("models/plane.3ds");
			break;
		case 2:
			m_player->setPlaneModel("models/plane.3ds");
			break;
		case 3:
			m_player->setPlaneModel("models/plane.3ds");
			break;
		}
	}

	Selector::on_accept(option);
}

PlayerSetupState::MapSelect::MapSelect(const Zeni::Point2f &upper_left_, const Zeni::Point2f &lower_right_, const Zeni::Point2f &expanded_upper_left_, const Zeni::Point2f &expanded_lower_right_, const Zeni::String &font_)
		: Selector(upper_left_, lower_right_, expanded_upper_left_, expanded_lower_right_, font_) {
			
	add_option("level1");
	add_option("level2");
	add_option("level3");

	select_option("level1");
	on_accept(get_selected());
}

void PlayerSetupState::MapSelect::on_accept(const Zeni::String &option) {

	level = option.std_str();

	Selector::on_accept(option);
}

void PlayerSetupState::Play_Button::on_accept() {
	Zeni::get_Game().push_state(new PlayState(players, level));
}

void PlayerSetupState::render() {
    Widget_Gamestate::render();

	Zeni::Vector2f carImageSize(100.0f, 50.0f);

	/*for (int i=0; i < players.size(); i++) {
		Zeni::Point2f carImagePosition(800.0f/(players.size())*i + 50.0f, 100.0f);
		Zeni::render_image(players[i]->getCarImage(), carImagePosition, carImagePosition + carImageSize);
	}
	if (trophyImage != "") {
		Zeni::render_image(trophyImage, Zeni::Point2f(700.0f,225.0f), Zeni::Point2f(750.0f,272.0f));
	}
	if (diagramImage != "") {
		Zeni::render_image(diagramImage, Zeni::Point2f(100.0f,300.0f), Zeni::Point2f(700.0f,600.0f));
	}*/
}