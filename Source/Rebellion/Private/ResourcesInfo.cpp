// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourcesInfo.h"
#include "UnrealNetwork.h"

AResourcesInfo::AResourcesInfo()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 1.0f;
}

int32 AResourcesInfo::GetGoldAmount() const
{
	return GoldAmount;
}

void AResourcesInfo::IncreaseGoldAmountBy(int32 Value)
{
	if (Value <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Negative value passed to increase gold"));
		return;
	}
	GoldAmount += Value;
}

void AResourcesInfo::DecreaseGoldAmountBy(int32 Value)
{
	if (Value <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Negative value passed to decrease gold"));
		return;
	}
	GoldAmount -= Value;
}

void AResourcesInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AResourcesInfo, GoldAmount);
}