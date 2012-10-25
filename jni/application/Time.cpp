#include "Time.h"

double Time::m_secondsPassed = 0;

double Time::getGameTime() {
	return m_secondsPassed;
}
void Time::updateGameTime(double timeStep) {
	m_secondsPassed += timeStep;
}