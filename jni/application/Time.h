class Time {
private:
	static double m_secondsPassed;
public:
	static double getGameTime();
	static void updateGameTime(double timeStep);
};