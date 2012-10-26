#include <zenilib.h>
#include "Player.h"
#include "Utils.h"

class PlayerSetupState : public Zeni::Widget_Gamestate {
  PlayerSetupState(const PlayerSetupState &);
  PlayerSetupState operator=(const PlayerSetupState &);

public:
	class ControlTypeSelect : public Zeni::Selector {
	//public:
		ControlTypeSelect(const ControlTypeSelect &);
		ControlTypeSelect & operator=(const ControlTypeSelect &);

		Player * m_player;

	public:
		ControlTypeSelect(Player * player, const Zeni::Point2f &upper_left_, const Zeni::Point2f &lower_right_, const Zeni::Point2f &expanded_upper_left_, const Zeni::Point2f &expanded_lower_right_, const Zeni::String &font_);

		void on_accept(const Zeni::String &option);
	};	

	class MapSelect : public Zeni::Selector {
	//public:
		MapSelect(const MapSelect &);
		MapSelect & operator=(const MapSelect &);

	public:
		MapSelect(const Zeni::Point2f &upper_left_, const Zeni::Point2f &lower_right_, const Zeni::Point2f &expanded_upper_left_, const Zeni::Point2f &expanded_lower_right_, const Zeni::String &font_);

		void on_accept(const Zeni::String &option);
	} m_mapSelect;	

	class MapSize : public Zeni::Slider {
	//public:
		MapSize(const MapSelect &);
		MapSize & operator=(const MapSelect &);

	public:
		MapSize(const Zeni::Point2f &endPointA, const Zeni::Point2f &endPointB, const float &sliderRadius);

		void on_accept();
	} m_mapSize;	

	class BuildingDensitySlider : public Zeni::Slider {
	//public:
		BuildingDensitySlider(const BuildingDensitySlider &);
		BuildingDensitySlider & operator=(const BuildingDensitySlider &);

	public:
		BuildingDensitySlider(const Zeni::Point2f &endPointA, const Zeni::Point2f &endPointB, const float &sliderRadius);

		void on_accept();
	} m_buildingDensitySlider;	

	class PowerupDensitySlider : public Zeni::Slider {
	//public:
		PowerupDensitySlider(const PowerupDensitySlider &);
		PowerupDensitySlider & operator=(const PowerupDensitySlider &);

	public:
		PowerupDensitySlider(const Zeni::Point2f &endPointA, const Zeni::Point2f &endPointB, const float &sliderRadius);

		void on_accept();
	} m_powerupDensitySlider;	

	class Play_Button : public Zeni::Text_Button {
		Play_Button(const Play_Button &);
		Play_Button & operator=(const Play_Button &);

		public:
		Play_Button()
			: Text_Button(Zeni::Point2f(200.0f, 225.0f), Zeni::Point2f(600.0f, 275.0f),
				"system_36_800x600", "Play") {
		}

		void on_accept();
	} play_button;

	PlayerSetupState();

	~PlayerSetupState();

	void on_uncover();

private:
	std::vector<ControlTypeSelect*> m_controls;

  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      Zeni::get_Game().pop_state();
  }

  void render();
};