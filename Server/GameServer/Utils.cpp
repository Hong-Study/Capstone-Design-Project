#include "pch.h"
#include "Utils.h"
#include "Player.h"

PlayerRef Utils::MakePlayerRef(GameSessionRef session)
{
	static atomic<uint64> playerCount = 0;

	PlayerRef player = make_shared<Player>();
	player->playerInfo->set_object_id(playerCount.fetch_add(1));

	player->session = session;
	session->player.store(player);

	return player;
}