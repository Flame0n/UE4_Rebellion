// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Actor.h"
#include "UnitAnimationBlueprint.generated.h"

/**
 * 
 */
UCLASS()
class REBELLION_API UUnitAnimationBlueprint : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	AActor* CachedOwner;

	UPROPERTY(BlueprintReadOnly)
	float Velocity;

	UPROPERTY(BlueprintReadOnly)
	bool IsDead;

	UPROPERTY(BlueprintReadOnly)
	bool IsAttacking;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeBeginPlay() override;

public:

	void SetIsAttacking(bool Attacking);

	void SetIsDead(bool Dead);

};