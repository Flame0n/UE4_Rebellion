// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UHealthComponent::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float Armor = BaseArmor + BonusArmor;
	float Multiplier = 1 - (0.052 * Armor) / (0.4 + 0.048 * Armor);
	float MultipliedDamage = Damage * Multiplier;

	if (MultipliedDamage > 0.f)
	{
		CurrentHealth -= MultipliedDamage;
		if (IsDead())
		{
			IStrategyUnitInterface* Unit = Cast<IStrategyUnitInterface>(GetOwner());
			Unit->Die();
		}
	}

	return MultipliedDamage;
}

bool UHealthComponent::IsDead() const
{
	return CurrentHealth <= 0.f;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::IncreaseArmor(float Armor) { 
	BonusArmor += Armor; 
}

void UHealthComponent::DecreaseArmor(float Armor) { 
	BonusArmor -= Armor; 
}