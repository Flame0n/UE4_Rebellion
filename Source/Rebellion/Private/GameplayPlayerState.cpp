// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayPlayerState.h"
#include "UnrealNetwork.h"

bool AGameplayPlayerState::IsEnemyToPlayer(APlayerState* Player) const
{
	if (Player == nullptr || TeamInfo == nullptr)
	{
		return false;
	}

	ITeamBelongInterface* OtherTeamBelongEntity = Cast<ITeamBelongInterface>(Player);
	if (OtherTeamBelongEntity == nullptr)
	{
		return false;
	}

	ATeamInfo* OtherTeam = OtherTeamBelongEntity->GetTeamInfo();

	return TeamInfo->IsEnemyToTeam(OtherTeam);
}

ATeamInfo * AGameplayPlayerState::GetTeamInfo() const
{
	return TeamInfo;
}

void AGameplayPlayerState::SetTeamInfo(ATeamInfo * Info)
{
	TeamInfo = Info;
}

AActor * AGameplayPlayerState::GetSpawnPoint() const
{
	if (SpawnPoint == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn point wasn't set"));
	}
	return SpawnPoint;
}

void AGameplayPlayerState::SetSpawnPoint(AActor* Point)
{
	if (Point == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Try to set empty spawn point"));
		return;
	}

	SpawnPoint = Point;
}

TSubclassOf<ACharacter> AGameplayPlayerState::GetCharacterAsset() const
{
	return CharacterAsset;
}

void AGameplayPlayerState::SetCharacterAsset(TSubclassOf<ACharacter> Asset)
{
	CharacterAsset = Asset;
}

AResourcesInfo * AGameplayPlayerState::GetResourcesInfo() const
{
	return ResourcesInfo;
}

void AGameplayPlayerState::SetResourcesInfo(AResourcesInfo * Info)
{
	ResourcesInfo = Info;
}

void AGameplayPlayerState::SetIsGameplayStarted(bool Value)
{
	IsGameplayStarted = Value;
}

bool AGameplayPlayerState::GetIsGameplayStarted() const {
	return IsGameplayStarted; 
}

void AGameplayPlayerState::AppendImprovementData(const FImprovementData & Data)
{
	CachedPlayerImprovementData += Data;

	TSet<IImprovableEntityInterface*> UnitsInTeam = TeamInfo->GetUnitsInTeam();
	for (IImprovableEntityInterface* Unit : UnitsInTeam)
	{
		Unit->AppendImprovementData(CachedPlayerImprovementData);
	}

	GetResourcesInfo()->IncreaseGoldAmountBy(Data.InstantGold);
}

void AGameplayPlayerState::DetachImprovementData(const FImprovementData & Data)
{
	CachedPlayerImprovementData -= Data;

	TSet<IImprovableEntityInterface*> UnitsInTeam = TeamInfo->GetUnitsInTeam();
	for (IImprovableEntityInterface* Unit : UnitsInTeam)
	{
		Unit->DetachImprovementData(CachedPlayerImprovementData);
	}
}

const FImprovementData& AGameplayPlayerState::GetImprovementData() const {
	return CachedPlayerImprovementData; 
}

void AGameplayPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameplayPlayerState, TeamInfo);
	DOREPLIFETIME(AGameplayPlayerState, ResourcesInfo);
	DOREPLIFETIME(AGameplayPlayerState, IsGameplayStarted);
	DOREPLIFETIME(AGameplayPlayerState, CachedPlayerImprovementData);
}

