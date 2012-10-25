#include <zenilib.h>
#include "PlayState.h"
#include "Input.h"
#include "Utils.h"
#include "Viewport.h"
#include "GameObject.h"
#include "Level.h"
#include "lib3ds.h"
#include "Sailplane.h"
#include "Building.h"
#include "Time.h"

PlayState::PlayState(std::vector<Player*> players, std::string level) {
    set_pausable(true);
	m_level = new Level(level);
	m_players = players;

	for (int i=0; i < 10; i++) {
		m_gameObjects.push_back(new Building(m_level->getPositionAtPoint(Zeni::Vector2f(m_level->getBounds().get_ij()).multiply_by(Zeni::Vector2f((float)(rand()%1000)/1000.0f, (float)(rand()%1000)/1000.0f))), Zeni::Quaternion(), Zeni::Model("models/buildingvert.3ds")));
	}
	
	Zeni::Vector2f screenSize = Zeni::Vector2f(Zeni::Point2f(Zeni::get_Video().get_render_target_size()));
	for (int i = 0; i < players.size(); i++) {
		Zeni::Vector3f levelBounds = m_level->getBounds();
		Zeni::Vector3f levelCenter(levelBounds - Zeni::Vector3f(levelBounds.i, levelBounds.j, 0.0f) * 0.5);
		m_gameObjects.push_back(players[i]->getNewPlane(levelCenter + (Zeni::Quaternion((float)(i)/(float)players.size() * Utils::PI*2.0f, 0.0f, 0.0f) * Zeni::Vector3f(1.0f, 0.0f, 0.0f)) * levelBounds.i));
		if (players.size() == 1) {
			m_viewports.push_back(new Viewport(players[i]->getLastPlane(), Zeni::Point2f(), Zeni::Vector2f(1.0f, 1.0f)));
		} else if (players.size() == 2) {
			m_viewports.push_back(new Viewport(players[i]->getLastPlane(), Zeni::Point2f((double)(i)/2.0f, 0), Zeni::Vector2f(0.5f, 1.0f)));
		} else if (players.size() > 2) {
			m_viewports.push_back(new Viewport(players[i]->getLastPlane(), Zeni::Point2f((double)(i%2)/2.0f, (double)(i/2)/2.0f), Zeni::Vector2f(0.5f, 0.5f)));
		}
	}

	m_chronometer.start();
	m_timePassed = 0.0;
}

PlayState::~PlayState() {
	for (int i=0; i < m_gameObjects.size(); i++) {
		delete m_gameObjects[i];
	}
	for (int i=0; i < m_viewports.size(); i++) {
		delete m_viewports[i];
	}
	delete m_level;
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

	Time::updateGameTime(timeStep);

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

	// Adjust viewports
	for (int i = 0; i < m_viewports.size(); i++) {
		m_viewports[i]->stepViewportPosition(timeStep);
	}

	applyStateModifications(stateModifications);
	Input::stepInput();

}

void PlayState::render() {
	Zeni::get_Video().set_clear_Color(Zeni::Color(0xFF7777FF));
	for (std::vector<Viewport*>::iterator it = m_viewports.begin(); it != m_viewports.end(); it++) {			
		(*it)->render(*m_level, m_gameObjects);
	}
}


const std::vector<std::vector<GameObject*>> PlayState::getGameObjectCollisions() {
	std::vector<std::vector<GameObject*>> collisions(m_gameObjects.size());
	std::vector< GameObject* > collidables;
	collidables.reserve(m_gameObjects.size());
	std::vector< GameObject* > sensors;
	sensors.reserve(m_gameObjects.size());

	for (int i = 0; i < m_gameObjects.size(); i++) {
		if (m_gameObjects[i]->willDetectCollisionsWithGameObjects()) {
			sensors.push_back(m_gameObjects[i]);
		} else if (m_gameObjects[i]->willCollideWithGameObjects()) {
			collidables.push_back(m_gameObjects[i]);
		}
	}

	for (int i = 0; i < sensors.size(); i++) {
		for (int j = i + 1; j < sensors.size(); j++) {
			if (sensors[i]->willCollideWithGameObjects() || sensors[j]->willCollideWithGameObjects()) {
				if (sensors[i]->isTouching(sensors[j])) {
					if (sensors[j]->willCollideWithGameObjects()) {
						collisions[i].push_back(sensors[j]);
					}
					if (sensors[i]->willCollideWithGameObjects()) {
						collisions[j].push_back(sensors[i]);
					}
				}
			}
		}
	}
	for (int i = 0; i < collidables.size(); i++) {
		for (int j = 0; j < sensors.size(); j++) {
			if (collidables[i]->isTouching(sensors[j])) {
				collisions[j].push_back(collidables[i]);
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
	for (std::list<GameObject*>::iterator it = stateModifications.gameObjectRemovals.begin(); it != stateModifications.gameObjectRemovals.end(); it++) {
		removeGameObject(*it);
	}
}