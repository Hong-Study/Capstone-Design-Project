#pragma once

#define WIN32_LEAN_AND_MEAN 

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "CorePch.h"

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

using GameSessionRef	= shared_ptr<class GameSession>;
using PlayerRef			= shared_ptr<class Player>;
using RoomRef			= shared_ptr<class Room>;

#define TIME_SYNC 1000/10

#include "ServerPacketHandler.h"
#include "Utils.h"
#include "GameSession.h"
#include "RoomManager.h"

#define SEND_PACKET(pkt)											 \
SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt); \
session->Send(sendBuffer);			