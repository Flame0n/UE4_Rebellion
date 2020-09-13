// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Controller.h"

#include "InvadeInfo.h"
#include "PlayerBelongInterface.h"

#include "InvadableComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REBELLION_API UInvadableComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	TArray<AInvadeInfo*> InvaderTeamsInfo;

	float BonusResistance = 0.f;

	void NotifyInvadeDone(APlayerState* Invader);

public:	
	// Sets default values for this component's properties
	UInvadableComponent();

	UPROPERTY(BlueprintReadOnly)
	float MaximumInvadePoints = 100;

	/** Apply invade to this actor. */
	float TakeInvade(float InvadePoints, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	void IncreaseResistance(float Value) { BonusResistance += Value; }

	void DecreaseResistance(float Value) { BonusResistance -= Value; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetInvadePointsByPlayer(APlayerState* Player) const;

private: 
		
	bool CharacterAlreadyInvadedBuilding(AActor* Character) const;
};
