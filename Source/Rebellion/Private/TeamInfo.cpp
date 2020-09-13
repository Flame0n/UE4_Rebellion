// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamInfo.h"
#include "GameplayPlayerState.h" // Should be here
#include "UnrealNetwork.h" // Should be here

ATeamInfo::ATeamInfo()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 1.0f;
}

uint8 ATeamInfo::GetTeamIndex() const
{
	return TeamIndex;
}

void ATeamInfo::SetTeamIndex(uint8 Index)
{
	TeamIndex = Index;
}

bool ATeamInfo::IsEnemyToTeam(ATeamInfo* Other) const
{
	if (Other == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Team info got null on enemy check"));
		return false;
	}

	if (TeamIndex == NO_TEAM || Other->TeamIndex == NO_TEAM)
	{
		return false;
	}

	return TeamIndex != Other->TeamIndex;
}

void ATeamInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ATeamInfo, TeamIndex, COND_InitialOnly);
}