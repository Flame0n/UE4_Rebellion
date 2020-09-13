// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitCharacter.h"
#include "GameFramework/Controller.h"
#include "Components/ActorComponent.h"
#include "InvadeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REBELLION_API UInvadeComponent : public UActorComponent
{
	GENERATED_BODY()

	AUnitCharacter* OwnerCharacter;

	const float InvadeTickrate = 0.2f;

	const float BaseInvadePower = 10.f * InvadeTickrate;

	float BonusInvadePower = 0.f;

public:	
	// Sets default values for this component's properties
	UInvadeComponent();

	UFUNCTION(BlueprintCallable)
	float PerformInvade(AActor* Target);

	void IncreaseInvadePower(float Power) { BonusInvadePower += Power; }

	void DecreaseInvadePower(float Power) { BonusInvadePower -= Power; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
