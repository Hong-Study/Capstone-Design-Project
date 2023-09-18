// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


struct PacketHeader
{
	uint16 size;
	uint16 id; // ��������ID (ex. 1=�α���, 2=�̵���û)
};

class SendBuffer : public TSharedFromThis<SendBuffer>
{
public:
	SendBuffer(int32 bufferSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer.GetData(); }
	int32 WriteSize() { return _writeSize; }
	int32 Capacity() { return static_cast<int32>(_buffer.Num()); }

	void CopyData(void* data, int32 len);
	void Close(uint32 writeSize);

private:
	TArray<BYTE>	_buffer;
	int32			_writeSize = 0;
};

#define USING_SHARED_PTR(name)	using name##Ref = TSharedPtr<class name>;

USING_SHARED_PTR(Session);
USING_SHARED_PTR(PacketSession);
USING_SHARED_PTR(SendBuffer);

#include "ClientPacketHandler.h"
#include "S1GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#define SEND_PACKET(Pkt)													\
SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);		\
Cast<US1GameInstance>(GWorld->GetGameInstance())->SendPacket(sendBuffer)	