#include "pch.h"
#include "RoomManager.h"
#include "Room.h"

shared_ptr<RoomManager> GRoom = make_shared<RoomManager>();

RoomManager::RoomManager()
{
	_rooms[1] = make_shared<Room>();
}

RoomManager::~RoomManager()
{
	_rooms.clear();
}

bool RoomManager::GmaeCreate(PlayerRef player)
{
	WRITE_LOCK;
	// TODO

	return true;
}

bool RoomManager::GameDelete(PlayerRef player, uint32 roomNum)
{
	WRITE_LOCK;
	// TODO

	return true;
}

bool RoomManager::GameInside(PlayerRef player, uint32 roomNum)
{
	{
		WRITE_LOCK;

		if (_rooms.find(roomNum) == _rooms.end())
			return false;
	}
	
	return _rooms[roomNum]->HandleEnterPlayerLocked(player);
}

bool RoomManager::GameOutside(PlayerRef player, uint32 roomNum)
{
	{
		WRITE_LOCK;

		if (_rooms.find(roomNum) == _rooms.find(roomNum))
			return false;
	}

	return _rooms[roomNum]->HandleLeavePlayerLocked(player);
}
