#include "pch.h"
#include "Room.h"
#include "Player.h"

Room::Room()
{
}

Room::~Room()
{
}

bool Room::HandleEnterPlayerLocked(PlayerRef player)
{
	WRITE_LOCK;

	bool success = EnterPlayer(player);

	GameSessionRef session = player->session.lock();
	if (session)
		return false;

	{
		Protocol::S_ENTER_GAME pkt;
		pkt.set_allocated_player(player->playerInfo);
		pkt.set_success(success);
		SEND_PACKET(pkt);

		pkt.release_player();
	}
}

bool Room::HandleLeavePlayerLocked(uint64 id)
{
	WRITE_LOCK;


	return false;
}

bool Room::EnterPlayer(PlayerRef player)
{
	uint64 playerId = player->playerInfo->object_id();

	if (_players.find(playerId) == _players.end())
		return false;

	_players.insert(make_pair(playerId, player));

	player->room.store(shared_from_this());
	
	return true;
}

bool Room::LeavePlayer(uint64 id)
{
	return false;
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 expectedId)
{

}
