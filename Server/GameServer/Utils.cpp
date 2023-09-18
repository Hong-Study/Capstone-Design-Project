#include "pch.h"
#include "Utils.h"
#include "Player.h"

PlayerRef Utils::MakePlayerRef(GameSessionRef session)
{
	static atomic<uint64> playerCount = 0;

	if (session == nullptr)
		return nullptr;

	PlayerRef player = make_shared<Player>();
	if (player)
		return nullptr;

	player->session = session;

	player->playerInfo->set_object_id(playerCount.fetch_add(1));

	return player;
}