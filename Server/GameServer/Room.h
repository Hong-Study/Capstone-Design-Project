#pragma once
#include "JobQueue.h"

class Room : public enable_shared_from_this<Room>
{
public:
	Room();
	virtual ~Room();

	bool HandleEnterPlayerLocked(PlayerRef player);
	bool HandleLeavePlayerLocked(uint64 id);

private:
	bool EnterPlayer(PlayerRef player);
	bool LeavePlayer(uint64 id);

	USE_LOCK;

private:
	void Broadcast(SendBufferRef sendBuffer, uint64 expectedId = 0);

private:
	unordered_map<uint64, PlayerRef> _players;
};

