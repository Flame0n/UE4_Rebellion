// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "AttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REBELLION_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

	/** Cache to increase performance */
	ACharacter* CachedOwnerCharacter;
	AController* CachedOwnerController;

	/** Basic damage that will be multiplied */
	const float BaseDamage = 20.f;

	/** Random damage range that willbe added or removed from calculated damage */
	const float AttackRandomDeviation = 5.f;

	/** Damage that will be added to BaseDamage */
	float BonusDamage = 0.f;

	/** Get damage type from owner */
	TSubclassOf<UDamageType> DamageType;

public:

	/** Sets default values for this component's properties */
	UAttackComponent();

	/** Called from owner to perform attack */
	UFUNCTION(BlueprintCallable)
	float PerformAttack(AActor* Target);

	void IncreaseDamage(float Damage);

	void DecreaseDamage(float Damage);

	/** Called from owner to set proper damage type */
	void SetDamageType(TSubclassOf<UDamageType> Type);

protected:

	/** Called when the game starts */
	virtual void BeginPlay() override;

public:

	/** Called every frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
