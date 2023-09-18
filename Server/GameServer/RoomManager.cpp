#include "pch.h"
#include "RoomManager.h"
#include "Room.h"

RoomManager::RoomManager()
{
	_rooms[1] = make_shared<Room>();
}

RoomManager::~RoomManager()
{
	_rooms.clear();
}

void RoomManager::GmaeCreate(PlayerRef player)
{
	WRITE_LOCK;
	// TODO
}

void RoomManager::GameDelete(PlayerRef player, uint32 roomNum)
{
	WRITE_LOCK;
	// TODO
}

void RoomManager::GameInside(PlayerRef player, uint32 roomNum)
{
	{
		WRITE_LOCK;

		if (_rooms.find(roomNum) == _rooms.find(roomNum))
			return;
	}
	
	_rooms[roomNum]->HandleEnterPlayerLocked(player);
}

void RoomManager::GameOutside(uint32 roomNum, uint64 playerId)
{
	{
		WRITE_LOCK;

		if (_rooms.find(roomNum) == _rooms.find(roomNum))
			return;
	}

	_rooms[roomNum]->HandleLeavePlayerLocked(playerId);
}
