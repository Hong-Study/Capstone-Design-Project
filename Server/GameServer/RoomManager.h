#pragma once

class Room;

class RoomManager
{
public:
	RoomManager();
	~RoomManager();

	void GmaeCreate(PlayerRef player);
	void GameDelete(PlayerRef player, uint32 roomNum);

	void GameInside(PlayerRef player, uint32 roomNum);
	void GameOutside(uint32 roomNum, uint64 playerId);

private:
	USE_LOCK;
	map<uint32, RoomRef> _rooms;
};

extern shared_ptr<RoomManager> GRoom;