// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackComponent.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedOwnerCharacter = Cast<ACharacter>(GetOwner());
	if (CachedOwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong owner character in Attack Component"));
		return;
	}

	CachedOwnerController = CachedOwnerCharacter->GetController();
}

void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UAttackComponent::PerformAttack(AActor * Target)
{
	if (!IsValid(Target))
	{
		UE_LOG(LogTemp, Log, TEXT("Target becomes invalid on attack"));
		return 0.f;
	}

	FDamageEvent DamageEvent(DamageType);

	float Damage = BaseDamage + BonusDamage + FMath::RandRange(-AttackRandomDeviation, AttackRandomDeviation);

	return Target->TakeDamage(Damage, DamageEvent, CachedOwnerController, CachedOwnerCharacter);
}

void UAttackComponent::IncreaseDamage(float Damage) { 
	BonusDamage += Damage; 
}

void UAttackComponent::DecreaseDamage(float Damage) {
	BonusDamage -= Damage; 
	if (BonusDamage < 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("Bonus damage became less than zero on %s"), *CachedOwnerCharacter->GetName())
	}
}

void UAttackComponent::SetDamageType(TSubclassOf<UDamageType> Type) { 
	DamageType = Type; 
}


