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
#include "Powerup.h"
#include "ResultsState.h"

PlayState::PlayState(std::vector<Player*> players, std::string level, const Zeni::Vector3f &levelSize, const double buildingDensity, const double powerupDensity) {
    set_pausable(true);
	m_level = new Level(level, levelSize);
	m_players = players;

	for (int i=0; i < 20 * buildingDensity; i++) {
		m_gameObjects.push_back(new Building(m_level->getPositionAtPoint(Zeni::Vector2f(m_level->getBounds().get_ij()).multiply_by(Zeni::Vector2f((float)(rand()%1000)/1000.0f, (float)(rand()%1000)/1000.0f))), Zeni::Quaternion(), Zeni::Model("models/buildingvert.3ds"), CollisionGeometry("models/building.col"), Zeni::Vector3f(200.0f, 200.0f, 200.0f + 800.0f * ((float)(rand()%1000)/1000.0f))));
	}
	for (int i=0; i < 20 * powerupDensity; i++) {
		m_gameObjects.push_back(new Powerup(m_level->getPositionAtPoint(Zeni::Vector2f(m_level->getBounds().get_ij()).multiply_by(Zeni::Vector2f((float)(rand()%1000)/1000.0f, (float)(rand()%1000)/1000.0f))) + Zeni::Vector3f(0.0f, 0.0f, m_level->getBounds().k), Powerup::Type(rand()%(int)(Powerup::Type::FINAL_ELEMENT))));
		for (int j=0; j < 100; j++) {
			bool touching = false;
			for (int k=0; k < m_gameObjects.size() - 1; k++) {
				if (m_gameObjects[k]->isTouching(m_gameObjects[m_gameObjects.size() - 1])) {
					touching = true;
					break;
				}
			}
			if (!touching) {
				break;
			} else {
				m_gameObjects[m_gameObjects.size() - 1]->setPosition(m_level->getPositionAtPoint(Zeni::Vector2f(m_level->getBounds().get_ij()).multiply_by(Zeni::Vector2f((float)(rand()%1000)/1000.0f, (float)(rand()%1000)/1000.0f))) + Zeni::Vector3f(0.0f, 0.0f, m_level->getBounds().k + 1000.0f * (float)(rand()%1000)/1000.0f));
			}
		}
	}
	
	Zeni::Vector2f screenSize = Zeni::Vector2f(Zeni::Point2f(Zeni::get_Video().get_render_target_size()));
	for (int i = 0; i < players.size(); i++) {
		Zeni::Vector3f levelBounds = m_level->getBounds();
		Zeni::Vector3f levelCenter(levelBounds - Zeni::Vector3f(levelBounds.i * 0.5, levelBounds.j * 0.5, -100.0f));
		m_gameObjects.push_back(players[i]->getNewPlane(levelCenter + (Zeni::Quaternion((float)(i)/(float)players.size() * Utils::PI*2.0f, 0.0f, 0.0f) * Zeni::Vector3f(1.0f, 0.0f, 0.0f)) * levelBounds.i, Zeni::Quaternion((float)(i)/(float)players.size() * Utils::PI*2.0f + Utils::PI, 0.0f, 0.0f)));
		if (players.size() == 1) {
			m_viewports.push_back(new Viewport(players[i]->getLastPlane(), Zeni::Point2f(), Zeni::Vector2f(1.0f, 1.0f)));
		} else if (players.size() == 2) {
			m_viewports.push_back(new Viewport(players[i]->getLastPlane(), Zeni::Point2f(0.0f, (double)(i)/2.0f), Zeni::Vector2f(1.0f, 0.5f)));
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
    Zeni::get_Game().joy_mouse.enabled = false;
}

void PlayState::on_pop() {
    //get_Window().mouse_grab(false);
    Zeni::get_Window().mouse_hide(false);
    Zeni::get_Game().joy_mouse.enabled = true;
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

	for (int i=0; i < m_players.size(); i++) {
		double distance = m_players[i]->getLastPlane()->getPosition().z - m_level->getPositionAtPoint(Zeni::Point2f(m_players[i]->getLastPlane()->getPosition())).z;
		if (abs(distance) < 20.0 || distance < 0.0) {
			if (m_players[i]->getLastPlane()->isTouching(m_level->getPlaneAtPoint(Zeni::Point2f(m_players[i]->getLastPlane()->getPosition()))) || distance < 0.0) {
				m_players[i]->getLastPlane()->damage(1);
			}
		}
	}

	if (m_timePassed >= 30) {
		std::vector<Sailplane::Stats> stats;
		for (int i = 0; i < m_players.size(); i++) {
			stats.push_back(m_players[i]->getLastPlane()->getStats());
		}
		Zeni::get_Game().pop_state();
		Zeni::get_Game().push_state(new ResultsState(m_players, stats));
	}

}

void PlayState::render() {
	Zeni::get_Video().set_clear_Color(Zeni::Color(0xFF7777FF));
	for (std::vector<Viewport*>::iterator it = m_viewports.begin(); it != m_viewports.end(); it++) {			
		(*it)->render(*m_level, m_gameObjects);
	}
}


const std::vector<std::vector<GameObject*>> PlayState::getGameObjectCollisions() {
	std::vector<std::vector<GameObject*>> collisions;
	collisions.resize(m_gameObjects.size());
	std::vector< GameObject* > collidables;
	collidables.reserve(m_gameObjects.size());
	std::vector< GameObject* > sensors;
	sensors.reserve(m_gameObjects.size());
	std::vector< int > sensorsIndex;
	sensorsIndex.reserve(m_gameObjects.size());

	for (int i = 0; i < m_gameObjects.size(); i++) {
		if (m_gameObjects[i]->willDetectCollisionsWithGameObjects()) {
			sensors.push_back(m_gameObjects[i]);
			sensorsIndex.push_back(i);
		} else if (m_gameObjects[i]->willCollideWithGameObjects()) {
			collidables.push_back(m_gameObjects[i]);
		}
	}

	for (int i = 0; i < sensors.size(); i++) {
		for (int j = i + 1; j < sensors.size(); j++) {
			if (sensors[i]->willCollideWithGameObjects() || sensors[j]->willCollideWithGameObjects()) {
				if (sensors[i]->isTouching(sensors[j])) {
					if (sensors[j]->willCollideWithGameObjects()) {
						collisions[sensorsIndex[i]].push_back(sensors[j]);
					}
					if (sensors[i]->willCollideWithGameObjects()) {
						collisions[sensorsIndex[j]].push_back(sensors[i]);
					}
				}
			}
		}
	}
	for (int i = 0; i < collidables.size(); i++) {
		for (int j = 0; j < sensors.size(); j++) {
			if (collidables[i]->isTouching(sensors[j])) {
				collisions[sensorsIndex[j]].push_back(collidables[i]);
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