#pragma once
#include "JobQueue.h"
class Room;

class RoomManager : public JobQueue
{
public:
	RoomManager();
	~RoomManager();

	bool GmaeCreate(PlayerRef player);
	bool GameDelete(PlayerRef player, uint32 roomNum);

	bool GameInside(PlayerRef player, uint32 roomNum);
	bool GameOutside(PlayerRef player, uint32 roomNum);

	void RoomSync();

private:
	USE_LOCK;
	map<uint32, RoomRef> _rooms;
};

extern shared_ptr<RoomManager> GRoom;