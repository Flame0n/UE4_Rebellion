// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectHouseComponent.h"
#include "GameplayGameMode.h"
#include "BuildingOutlineComponent.h"
#include "MatchStateNotifiable.h"
#include "GameFramework/PlayerController.h"
#include "GameplayPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class REBELLION_API AGameplayPlayerController : public APlayerController, public IMatchStateNotifiable
{
	GENERATED_BODY()

private:

	/** Controller components */
	USelectHouseComponent* SelectHouseComponent;
	UBuildingOutlineComponent* BuildingOutlineComponent;

	/** Buildings trace channel */
	TEnumAsByte<ETraceTypeQuery> BuildingTraceType;

	/** Mouse click handler */
	UFUNCTION()
	void OnMouseClick();

	/** RPC call to spawn squad on server */
	UFUNCTION(Server, Reliable, WithValidation)
	void RPCSpawnSquad(AActor* Destination, AGameplayPlayerController* Controller);

	UFUNCTION(BlueprintCallable)
	void ClientSpawnSquad();

public:

	AGameplayPlayerController();

	/** Asset of indictor that appears on selected building */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Assets")
	TSubclassOf<AActor> RedArrowAsset;

	/** Asset of indictor that appears on selected building */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Assets")
	TSubclassOf<AActor> BlueArrowAsset;

	/** Asset of collision category */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Assets")
	TEnumAsByte<ECollisionChannel> BuildingCollisionAsset;

	virtual void NotifyMatchWasEnded(APlayerState* Winner) override;

	UFUNCTION(Client, Reliable)
	void ClientMatchWasEnded(bool IsWin);

protected:

	/** Called when controller recieves pawn on server */
	virtual void OnPossess(APawn* aPawn) override;

	/** Called when controller recieves pawn on client */
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void EventMatchWasEnded(bool IsWin);
};
