// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/OnlineEngineInterface.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameSession.h"

#include "MatchStateNotifiable.h"

#include "GameplayGameSession.generated.h"

class AGameplayPlayerState;

/**
 * 
 */
UCLASS()
class REBELLION_API AGameplayGameSession : public AGameSession
{
	GENERATED_BODY()

	TSet<APlayerController*> ConnectedPlayers;
	
public:

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void KickAllPlayers();

	void HandleMatchWasEnded(APlayerState* Winner);
};
