#include <zenilib.h>
#include "PlayState.h"
#include "Input.h"
#include "Utils.h"

PlayState::PlayState() : m_camera(Zeni::Point3f(0,0,100)), m_level("level1") {
    set_pausable(true);
	m_gameObjects.push_back(new GameObject());
	m_chronometer.start();
	m_timePassed = 0.0;
	m_camera.tunnel_vision_factor = 1.5f;
	m_camera.set_fov_deg(80.0f);
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

	StateModifications stateModifications = StateModifications();

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
	std::vector< std::vector<GameObject*> > collisions = getGameObjectCollisions();
	for (int i=0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i]->handleCollisions(collisions[i]);
		stateModifications.combine(m_gameObjects[i]->act(collisions[i]));
	}

	applyStateModifications(stateModifications);

	/*Utils::printDebugMessage(sp.nearest_point(pl).second);
	Utils::printDebugMessage("\n");*/
}

void PlayState::render() {
	Zeni::get_Video().set_3d(m_camera);
	Zeni::Light light = Zeni::Light(Zeni::Color(1.0, 1.0, 1.0, 1.0));
	light.position = Zeni::Point3f(10, 10, 10);
	light.set_spot_phi(Utils::PI/6.0);
	light.set_light_type(Zeni::LIGHT_DIRECTIONAL);
	Zeni::get_Video().set_lighting(true);
	Zeni::get_Video().set_Light(0, light);

	m_level.render();

	for (int i=0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i]->render();
	}

	Zeni::get_Video().set_lighting(false);
}


const std::vector<std::vector<GameObject*>> PlayState::getGameObjectCollisions() {
	std::vector<std::vector<GameObject*>> collisions(m_gameObjects.size());
	for (int i = 0; i < m_gameObjects.size(); i++) {
		if (m_gameObjects[i]->willDetectCollisionsWithGameObjects()) {
			for (int j = i + 1; j < m_gameObjects.size(); j++) {
				if (m_gameObjects[i]->isTouching(m_gameObjects[j])) {
					collisions[i].push_back(m_gameObjects[j]);
					if (m_gameObjects[j]->willDetectCollisionsWithGameObjects()) {
						collisions[j].push_back(m_gameObjects[i]);
					}
				}
			}
		}
	}
	return collisions;
}

void PlayState::addGameObject(GameObject * object) {
	m_gameObjects.push_back(object);
}
	
void PlayState::removeGameObject(GameObject * object) {
	GameObject* removalObject = nullptr;
	for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++) {
		if (*it == object) {
			GameObject* removalObject = *it;
			m_gameObjects.erase(it);
			break;
		}
	}
	if (removalObject != nullptr) {
		delete removalObject;
	}
}

void PlayState::applyStateModifications(StateModifications &stateModifications) {
	for (std::list<GameObject*>::iterator it = stateModifications.gameObjectAdditions.begin(); it != stateModifications.gameObjectAdditions.end(); it++) {
		addGameObject(*it);
	}
	for (std::list<GameObject*>::iterator it = stateModifications.gameObjectAdditions.begin(); it != stateModifications.gameObjectAdditions.end(); it++) {
		removeGameObject(*it);
	}
}