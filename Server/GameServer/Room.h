#pragma once
#include "JobQueue.h"

struct Point
{
	float x;
	float y;
	float z;
};

class Room : public enable_shared_from_this<Room>
{
public:
	Room();
	virtual ~Room();

	bool HandleEnterPlayerLocked(PlayerRef player);
	bool HandleLeavePlayerLocked(PlayerRef player);

private:
	bool EnterPlayer(PlayerRef player);
	bool LeavePlayer(uint64 id);

	USE_LOCK;

private:
	void Broadcast(SendBufferRef sendBuffer, uint64 expectedId = 0);

private:
	unordered_map<uint64, PlayerRef>	_players;
	vector<Point>						_spawnPoint;
};

