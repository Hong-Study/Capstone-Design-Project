#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "Player.h"
#include "Room.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	// TODO : DB에서 Account 정보를 긁어온다.
	Protocol::S_LOGIN loginPkt;

	loginPkt.set_success(true);
	SEND_PACKET(loginPkt);

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	cout << "Player was " << pkt.room_index() << " Room Inside" << endl;
	PlayerRef player = Utils::MakePlayerRef(static_pointer_cast<GameSession>(session));

	return GRoom->GameInside(player, pkt.room_index());
}

bool Handle_C_CREATE_GAME(PacketSessionRef& session, Protocol::C_CREATE_GAME& pkt)
{
	return false;
}

bool Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	cout << "Player was " << player->playerInfo->object_id() << " Room OutSide" << endl;

	return room->HandleLeavePlayerLocked(player);
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	return true;
}
