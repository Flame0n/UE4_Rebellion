// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "ResourcesInfo.h"
#include "TeamInfo.h"
#include "TeamBelongInterface.h"
#include "ImprovableEntityInterface.h"

#include "GameplayPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class REBELLION_API AGameplayPlayerState : public APlayerState, public IImprovableEntityInterface, public ITeamBelongInterface
{
	GENERATED_BODY()

private:

	/** All bonuses from invaded buildings. Needs to display in GUI */
	UPROPERTY(Replicated)
	FImprovementData CachedPlayerImprovementData;

	/** Initial spawn point of this player */
	AActor* SpawnPoint;

	/** Asset that linked from blueprints */
	TSubclassOf<ACharacter> CharacterAsset;

	/** Information about team of this player */
	UPROPERTY(Replicated)
	ATeamInfo* TeamInfo;

	/** Information about this player reources */
	UPROPERTY(Replicated)
	AResourcesInfo* ResourcesInfo;

	UPROPERTY(Replicated)
	bool IsGameplayStarted = false;

public:

	virtual bool IsEnemyToPlayer(APlayerState* Player) const override;

	UFUNCTION(BlueprintCallable)
	virtual ATeamInfo* GetTeamInfo() const override;

	/** Apply team to player */
	void SetTeamInfo(ATeamInfo* Info);

	/** Store initial spawn point of player */
	AActor* GetSpawnPoint() const;
	void SetSpawnPoint(AActor* Point);

	/** Apply single character asset to this player */
	TSubclassOf<ACharacter> GetCharacterAsset() const;
	void SetCharacterAsset(TSubclassOf<ACharacter> Asset);

	UFUNCTION(BlueprintCallable)
	AResourcesInfo* GetResourcesInfo() const;

	void SetResourcesInfo(AResourcesInfo* Info);

	void SetIsGameplayStarted(bool Value);

	UFUNCTION(BlueprintCallable)
	bool GetIsGameplayStarted() const;

	virtual void AppendImprovementData(const FImprovementData& Data) override;

	virtual void DetachImprovementData(const FImprovementData& Data) override;

	UFUNCTION(BlueprintCallable)
	const FImprovementData& GetImprovementData() const;
};
