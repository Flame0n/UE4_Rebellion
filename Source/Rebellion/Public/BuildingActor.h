// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ImprovableEntityInterface.h"
#include "PlayerBelongInterface.h"

#include "BuildingActor.generated.h"

class AGameplayGameMode;
class UTeamComponent;
class UInvadableComponent;

UCLASS()
class REBELLION_API ABuildingActor : public AActor, public IImprovableEntityInterface, public IPlayerBelongInterface
{
	GENERATED_BODY()

	/** Building components */
	UTeamComponent* TeamComponent;
	UInvadableComponent* InvadableComponent;

	/** Flag that will be spawned locally when player invades this building */
	AActor* LocalInvadeFlag;
	
public:

	/** Asset of local flag */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Assets")
	TSubclassOf<AActor> FlagAsset;

	/** The ability of building to resist invade */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float BaseResistance = 0.f;

	/** Sets default values for this actor's properties */
	ABuildingActor();

	/** Called when character receive damage */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Increase building stats */
	virtual void AppendImprovementData(const FImprovementData& Data) override;

	/** Decrease building stats */
	virtual void DetachImprovementData(const FImprovementData& Data) override;

	virtual bool IsEnemyToActor(AActor* Actor) const override;

	virtual void SetOwnerPlayer(APlayerState* State) override;

	virtual APlayerState* GetOwnerPlayer() const override;

protected:

	/** Defines what bonuses building provides to player when being invaded */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FImprovementData ProvidedImprovementData;

public:	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called when somebody invades this */
	void HandleInvadeDone(APlayerState* Player);

	/** Called from server when owner ofthis building changes */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastClientGotInvade(APlayerState* Player);

	const FImprovementData& GetProvidedImprovementData() const;

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

};
