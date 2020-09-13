// Fill out your copyright notice in the Description page of Project Settings.


#include "InvadeInfo.h"
#include "UnrealNetwork.h"

AInvadeInfo::AInvadeInfo()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 1.0f;
}

void AInvadeInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInvadeInfo, Team);
	DOREPLIFETIME(AInvadeInfo, InvadePoints);
}