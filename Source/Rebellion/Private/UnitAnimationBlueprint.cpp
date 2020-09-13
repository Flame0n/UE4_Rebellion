// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAnimationBlueprint.h"
#include "UnitCharacter.h" // Should be here

void UUnitAnimationBlueprint::NativeUpdateAnimation(float DeltaSeconds)
{
	if (CachedOwner == nullptr)
	{
		return;
	}

	Velocity = CachedOwner->GetVelocity().Size();
}

void UUnitAnimationBlueprint::NativeBeginPlay()
{
	AActor* UnitCharacter = TryGetPawnOwner();
	if (UnitCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid owner in animation blueprint"));
		return;
	}
	CachedOwner = UnitCharacter;
}

void UUnitAnimationBlueprint::SetIsAttacking(bool Attacking) {
	IsAttacking = Attacking; 
}

void UUnitAnimationBlueprint::SetIsDead(bool Dead) {
	IsDead = Dead; 
}