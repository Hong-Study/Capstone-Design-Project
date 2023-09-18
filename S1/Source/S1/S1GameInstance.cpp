// Fill out your copyright notice in the Description page of Project Settings.


#include "S1GameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Network/Protocol.pb.h"
#include "ClientPacketHandler.h"
#include "Player/MyCharacter.h"
#include "Player/AnotherCharacter.h"

void US1GameInstance::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// Session
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();

		// TEMP : Lobby에서 캐릭터 선택 창 등
		{
			Protocol::C_LOGIN pkt;
			SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
			SendPacket(SendBuffer);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void US1GameInstance::DisconnectFromGameServer()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SEND_PACKET(LeavePkt);

	//protocol::c_leave_game pkt;
	/*if (socket)
	{
		isocketsubsystem* socketsubsystem = isocketsubsystem::get();
		socketsubsystem->destroysocket(socket);
		socket = nullptr;
	}*/
}

void US1GameInstance::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void US1GameInstance::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void US1GameInstance::HandleSpawn(const Protocol::PlayerInfo& PlayerInfo)
{
	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = PlayerInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Input : %d"), ObjectId);

	FVector SpawnLocation(PlayerInfo.x(), PlayerInfo.y(), PlayerInfo.z());

	AActor* Player = nullptr;
	if (MyObjectId == ObjectId)
	{
		Player = World->SpawnActor(AMyCharacter::StaticClass(), &SpawnLocation);
	}
	else
	{
		Player = World->SpawnActor(AAnotherCharacter::StaticClass(), &SpawnLocation);
	}

	if (Player == nullptr)
		return;

	Players.Add(ObjectId, Player);
}

void US1GameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	MyObjectId = EnterGamePkt.player().object_id();

	HandleSpawn(EnterGamePkt.player());
}

void US1GameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& PlayerInfo : SpawnPkt.players())
	{
		HandleSpawn(PlayerInfo);
	}
}

void US1GameInstance::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	AActor** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	World->DestroyActor(*FindActor);
}

void US1GameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}
