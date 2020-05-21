#pragma once

class IPathFinder;
class WowGame;

class WowBot
{
public:
	WowBot(WowGame& game);

	~WowBot();

	void run();

	const WowGame& getGame() const;

	void setBotStarted(bool started);

	void loadLinearWaypoints(const std::vector<Vector3f>& waypoints);

protected:
	WowGame& mGame;
	bool mBotStarted = false;
	std::unique_ptr<IPathFinder> mPathFinder;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowBot& obj
	)
{
	out << "[WowBot]";
	// TODO print navigator state
	return out;
}
