#include <zenilib.h>
#include "PlayState.h"
#include "Input.h"
#include "Utils.h"

PlayState::PlayState() : m_camera(Zeni::Point3f(0,0,100)), m_level("level1") {
    set_pausable(true);
	m_gameObjects.push_back(new GameObject());
	m_chronometer.start();
	m_timePassed = 0.0;
}

void PlayState::on_push() {
    //get_Window().mouse_grab(true);
    Zeni::get_Window().mouse_hide(true);
    //get_Game().joy_mouse.enabled = false;
}

void PlayState::on_pop() {
    //get_Window().mouse_grab(false);
    Zeni::get_Window().mouse_hide(false);
    //get_Game().joy_mouse.enabled = true;
}

void PlayState::on_key(const SDL_KeyboardEvent &event) {
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		Input::updateKey(event.keysym.sym, event.type == SDL_KEYDOWN);
	}
	Gamestate_Base::on_key(event);
}

void PlayState::on_joy_button(const SDL_JoyButtonEvent &event) {
	if (event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP) {
		Input::updateJoyButton(event.which, event.button, event.type == SDL_JOYBUTTONDOWN);
	}
	Gamestate_Base::on_joy_button(event);
}

void PlayState::on_joy_axis(const SDL_JoyAxisEvent &event) {
	Input::updateJoyAxisValue(event.which, event.axis, event.value);
	Gamestate_Base::on_joy_axis(event);
}

void PlayState::perform_logic() {
    const float timePassed = m_chronometer.seconds();
	const float timeStep = std::min(timePassed - m_timePassed, 50.0f/1000.0f); // Set lower bound on simulation at 20 fps
    m_timePassed = timePassed;

	Input::stepInput();
	/////
	if (Input::isKeyDown(SDLK_LEFT)) {
		m_camera.turn_left_xy(.05);
	}
	if (Input::isKeyDown(SDLK_RIGHT)) {
		m_camera.turn_left_xy(-.05);
	}
	if (Input::isKeyDown(SDLK_UP)) {
		m_camera.move_forward_xy(2.0);
	}
	if (Input::isKeyDown(SDLK_DOWN)) {
		m_camera.move_forward_xy(-2.0);
	}

	Zeni::Collision::Plane pl  = Zeni::Collision::Plane(Zeni::Point3f(100,100,100), Zeni::Vector3f(0, -1, 0));

	Zeni::Collision::Line_Segment sp = Zeni::Collision::Line_Segment(m_camera.position, m_camera.position + Zeni::Vector3f(0,20,0));
	////
	// Run physics
	for (int i=0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i]->stepPhysics(timeStep);
	}

	// Handle collisions and perform actions
	for (int i=0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i]->handleCollisions();
		m_gameObjects[i]->act();
	}

	/*Utils::printDebugMessage(sp.nearest_point(pl).second);
	Utils::printDebugMessage("\n");*/
}

void PlayState::render() {
	Zeni::get_Video().set_3d(m_camera);

	m_level.render();

	for (int i=0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i]->render();
	}
}