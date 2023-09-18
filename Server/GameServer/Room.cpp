#include "pch.h"
#include "Room.h"
#include "Player.h"

Room::Room()
{
	for (int i = 0; i < 50 ; i++)
	{
		_spawnPoint.push_back(Point(i * 30.f, i * 30.f, 88.f ));
	}
}

Room::~Room()
{

}

bool Room::HandleEnterPlayerLocked(PlayerRef player)
{
	WRITE_LOCK;

	bool success = EnterPlayer(player);

	if (success == false)
		return false;

	uint64 count = static_cast<int32>(player->playerInfo->object_id());

	player->playerInfo->set_x(_spawnPoint[count].x);
	player->playerInfo->set_y(_spawnPoint[count].y);
	player->playerInfo->set_z(_spawnPoint[count].z);
	player->playerInfo->set_yaw(Utils::GetRandom(0.f, 100.f));

	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(*player->playerInfo);
		enterGamePkt.set_allocated_player(playerInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	{
		Protocol::S_SPAWN spawnPkt;

		Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
		playerInfo->CopyFrom(*player->playerInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, player->playerInfo->object_id());
	}

	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _players)
		{
			Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom(*item.second->playerInfo);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	return success;
}

bool Room::HandleLeavePlayerLocked(PlayerRef player)
{
	if (player == nullptr)
		return false;

	WRITE_LOCK;

	const uint64 objectId = player->playerInfo->object_id();
	bool success = LeavePlayer(objectId);

	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);

		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	return success;
}


bool Room::EnterPlayer(PlayerRef player)
{
	uint64 playerId = player->playerInfo->object_id();

	if (_players.find(playerId) != _players.end())
		return false;

	_players.insert(make_pair(playerId, player));

	player->room.store(shared_from_this());
	
	return true;
}

bool Room::LeavePlayer(uint64 objectId)
{
	if (_players.find(objectId) == _players.end())
		return false;

	PlayerRef player = _players[objectId];
	player->room.store(weak_ptr<Room>());

	_players.erase(objectId);

	return true;
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 expectedId)
{
	for (auto& playerPairs : _players)
	{
		if (playerPairs.first == expectedId)
			continue;

		if (auto session = playerPairs.second->session.lock())
			session->Send(sendBuffer);
	}
}
