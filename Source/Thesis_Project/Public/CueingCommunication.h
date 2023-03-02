// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CueingCommunication.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THESIS_PROJECT_API UCueingCommunication : public UActorComponent
{
	GENERATED_BODY()
	const FString TheIP = "127.0.0.1";

	TSharedPtr<FInternetAddr> RemoteAddrData;
	FSocket* SenderDataSocket;
	const int32 TheDataPort = 22608;

	TSharedPtr<FInternetAddr> RemoteAddrCtrl;
	FSocket* SenderCtrlSocket;
	const int32 TheCtrlPort = 22609;
	char dataControl[5]{}; //"QUIT" "CONN" "STRT" "STOP" "STAT"
	bool firstRun;

public:	
	// Sets default values for this component's properties
	UCueingCommunication();

	bool SetupDataUDPSender();
	bool SetupCtrlUDPSender();

	UFUNCTION(BlueprintCallable)
		bool SendFloatArray(TArray<float> data);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	bool Send(FArrayWriter Writer);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;		
};
