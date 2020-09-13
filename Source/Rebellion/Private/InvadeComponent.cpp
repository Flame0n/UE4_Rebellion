// Fill out your copyright notice in the Description page of Project Settings.


#include "InvadeComponent.h"

UInvadeComponent::UInvadeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UInvadeComponent::PerformInvade(AActor * Target)
{
	AController* OwnerController = OwnerCharacter->GetController();

	if (!IsValid(Target))
	{
		return 0.f;
	}

	FDamageEvent DamageEvent(OwnerCharacter->InvadeDamageType);
	float InvadePower = BaseInvadePower + BonusInvadePower;

	return Target->TakeDamage(InvadePower, DamageEvent, OwnerController, OwnerCharacter);
}

void UInvadeComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AUnitCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong owner in Invade Component"));
	}
}

void UInvadeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

