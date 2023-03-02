// Fill out your copyright notice in the Description page of Project Settings.


#include "CueingCommunication.h"

// Sets default values for this component's properties
UCueingCommunication::UCueingCommunication()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SenderDataSocket = NULL;
	SenderCtrlSocket = NULL;

	firstRun = true;


	// ...
}


// Called when the game starts
void UCueingCommunication::BeginPlay()
{
	Super::BeginPlay();

	//Setup socket
	SetupDataUDPSender();
	SetupCtrlUDPSender();
	
}


// Called every frame
void UCueingCommunication::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (firstRun)
	{
		//Send ctrl sig: CONN
		strcpy(dataControl, "CONN");
		int32 BytesSent = 0;
		SenderCtrlSocket->SendTo((uint8*)dataControl, sizeof(dataControl), BytesSent, *RemoteAddrCtrl);
		if (BytesSent <= 0)
		{
			const FString Str = "CONN SIG: Ctrl Socket is valid but the receiver received 0 bytes, make sure it is listening properly!";
			UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
			return;
		}
		firstRun = false;
	}

	//Send ctrl sig: STRT
	strcpy(dataControl, "STRT");
	int32 BytesSent = 0;
	SenderCtrlSocket->SendTo((uint8*)dataControl, sizeof(dataControl), BytesSent, *RemoteAddrCtrl);
	if (BytesSent <= 0)
	{
		const FString Str = "CONN SIG: Ctrl Socket is valid but the receiver received 0 bytes, make sure it is listening properly!";
		UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		return;
	}

	//Serialize and send data in Blueprint
}


void UCueingCommunication::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (!SenderCtrlSocket) {
		return;
	}

	//send ctrl signal
	//stop
	strcpy(dataControl, "STOP");
	int32 BytesSent = 0;
	SenderCtrlSocket->SendTo((uint8*)dataControl, sizeof(dataControl), BytesSent, *RemoteAddrCtrl);
	if (BytesSent <= 0)
	{
		const FString Str = "STOP SIG: Ctrl Socket is valid but the receiver received 0 bytes, make sure it is listening properly!";
		UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		return;
	}
	//disc
	BytesSent = 0;
	strcpy(dataControl, "DISC");
	SenderCtrlSocket->SendTo((uint8*)dataControl, sizeof(dataControl), BytesSent, *RemoteAddrCtrl);
	if (BytesSent <= 0)
	{
		const FString Str = "DISC SIG: Ctrl Socket is valid but the receiver received 0 bytes, make sure it is listening properly!";
		UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		return;
	}

	//quit
	BytesSent = 0;
	strcpy(dataControl, "QUIT");
	SenderCtrlSocket->SendTo((uint8*)dataControl, sizeof(dataControl), BytesSent, *RemoteAddrCtrl);
	if (BytesSent <= 0)
	{
		const FString Str = "QUIT SIG: Ctrl Socket is valid but the receiver received 0 bytes, make sure it is listening properly!";
		UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		return;
	}


	//clean up socket
	if (SenderDataSocket) {
		SenderDataSocket->Close();
		ISocketSubsystem::Get()->DestroySocket(SenderDataSocket);
	}

	if (SenderCtrlSocket) {
		SenderCtrlSocket->Close();
		ISocketSubsystem::Get()->DestroySocket(SenderCtrlSocket);
	}
}



//

bool UCueingCommunication::SetupDataUDPSender()
{

	//const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, bool UDP
	//Data socket
	RemoteAddrData = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool bIsValid;
	RemoteAddrData->SetIp(*TheIP, bIsValid);
	RemoteAddrData->SetPort(TheDataPort);

	if (!bIsValid) {
		return false;
	}

	SenderDataSocket = FUdpSocketBuilder("DataSocket").AsReusable().WithBroadcast();

	int32 SendSize = 2 * 1024 * 1024;
	SenderDataSocket->SetSendBufferSize(SendSize, SendSize);
	SenderDataSocket->SetReceiveBufferSize(SendSize, SendSize);

	return true;
}

bool UCueingCommunication::SetupCtrlUDPSender()
{
	// const FString & YourChosenSocketName, const FString& TheIP, const int32 ThePort, bool UDP
		//Data socket
		RemoteAddrCtrl = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool bIsValid;
	RemoteAddrCtrl->SetIp(*TheIP, bIsValid);
	RemoteAddrCtrl->SetPort(TheCtrlPort);

	if (!bIsValid) {
		return false;
	}

	SenderCtrlSocket = FUdpSocketBuilder("CtrlSocket").AsReusable().WithBroadcast();

	int32 SendSize = 2 * 1024 * 1024;
	SenderCtrlSocket->SetSendBufferSize(SendSize, SendSize);
	SenderCtrlSocket->SetReceiveBufferSize(SendSize, SendSize);

	return true;
}


bool UCueingCommunication::SendFloatArray(TArray<float> data)
{
	FArrayWriter write;
	for (int i = 0; i < data.Num(); ++i) {
		write << data[i];
	}
	return Send(write);
}



bool UCueingCommunication::Send(FArrayWriter Writer)
{
	if (!SenderDataSocket) {
		return false;
	}

	int32 BytesSent = 0;

	SenderDataSocket->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *RemoteAddrData);

	if (BytesSent <= 0)
	{
		const FString Str = "Socket is valid but the receiver received 0 bytes, make sure it is listening properly!";
		UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		return false;
	}
	return true;
}
