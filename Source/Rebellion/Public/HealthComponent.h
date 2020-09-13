// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Controller.h"

#include "StrategyUnitInterface.h"

#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REBELLION_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	/** Base armor amount for this component */
	const float BaseArmor = 0.f;

	/** Bonus armor that will be added to BaseArmor */
	float BonusArmor = 0.f;

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	/** Initial health of units */
	UPROPERTY(BlueprintReadOnly)
	float MaximumHealth = 100;

	/** Current unit health */
	UPROPERTY(BlueprintReadOnly, Replicated)
	float CurrentHealth = MaximumHealth;

	/** Apply damage to this actor. */
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	/** Check whether the unit is dead */
	bool IsDead() const;

	void IncreaseArmor(float Armor);

	void DecreaseArmor(float Armor);

protected:

	/** Called when the game starts */
	virtual void BeginPlay() override;

public:

	/** Called every frame */
 	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
