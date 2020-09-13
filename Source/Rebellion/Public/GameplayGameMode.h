// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TeamInfo.h"
#include "ResourcesInfo.h"
#include "UnitCharacter.h"
#include "UnitAIController.h"
#include "GameplayPlayerState.h"
#include "GameplayComputerAIController.h"
#include "BuildingActor.h"
#include "GameFramework/GameMode.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameplayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class REBELLION_API AGameplayGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	/** How many players should be in the map to start game */
	static const int32 NUM_PLAYERS_TO_START_GAME = 2;

	/** Time to wait for players */
	static const int32 TIME_BEFORE_GAME_STARTS = 5;

	/** How much gold each player has on start */
	static const int32 INITIAL_GOLD_AMOUNT = 100;

	/** How much gold adds on timer tick */
	static const int32 ADD_GOLD_RATE = 1;

	/** How much units should be spawned when SpawnSquad() called */
	static const uint8 UNITS_IN_SQUAD = 4;

	/** How much costs one squad */
	static const int32 SQUAD_PRICE = 80;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Assets")
	TSubclassOf<AActor> SwordAsset;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Assets")
	TSubclassOf<AActor> TorchAsset;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Assets")
	TSubclassOf<ACharacter> CharacterAsset;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Assets")
	TSubclassOf<ACharacter> PatrolCharacterAsset;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Assets")
	TSubclassOf<AActor> CastleClassAsset;

private:

	/** Array of computer-driven controllers */
	TArray<AGameplayComputerAIController*> ComputerAIControllers;

	/** Delegate to count when gameplay has been started */
	FTimerHandle StartGameplayTimerHandle;

	/** Handle and delegate for increase gold timer */
	FTimerDelegate AddGoldTimerDelegate;
	FTimerHandle AddGoldTimerHandle;

	/** Does player win */
	bool IsPlayerWin = false;

public:

	/** Default constructor */
	AGameplayGameMode(const FObjectInitializer & ObjectInitializer);

	/** Choose player for the player */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	/** Fired on player success login */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Fired when game started */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual bool ReadyToEndMatch_Implementation() override;

	virtual void HandleMatchHasEnded() override;

	/** Spawns squad in place defined in controller */
	void SpawnSquad(AActor* Destination, AController* PlayerController);

	/** Checkes whether game can be started */
	UFUNCTION()
	void StartGameplayTimerTick();

	/** Adds gold to all connected players*/
	UFUNCTION()
	void AddGoldToAllPlayers(int32 Amount);

	void OnInvadeDone(APlayerState* Player, ABuildingActor* Building);

private:

	/** Assing default team info for buildings */
	void AssignDefaultTeamsToBuildings(APlayerState* State);

	/** Creates virtual player on server and spawn resources info for him */
	AController* CreateVirtualPlayer();

	/** Spawns item in specified socket of character */
	void SpawnItemFor(ACharacter* Character, TSubclassOf<AActor> Asset, FName SocketName);

	/** Starts gameplay mode */
	void StartGameplay();

	void WinGame(APlayerState* PlayerState);
};