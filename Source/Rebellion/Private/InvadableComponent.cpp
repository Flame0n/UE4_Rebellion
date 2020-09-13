// Fill out your copyright notice in the Description page of Project Settings.


#include "InvadableComponent.h"
#include "BuildingActor.h"
#include "UnrealNetwork.h"

UInvadableComponent::UInvadableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInvadableComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInvadableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInvadableComponent::NotifyInvadeDone(APlayerState * Invader)
{
	UE_LOG(LogTemp, Log, TEXT("Player %s has invade %s"), *Invader->GetName(), *GetOwner()->GetName());
	ABuildingActor* Building = Cast<ABuildingActor>(GetOwner());
	Building->HandleInvadeDone(Invader);
}

float UInvadableComponent::TakeInvade(float InvadePoints, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CharacterAlreadyInvadedBuilding(DamageCauser) || InvadePoints <= 0.f)
	{
		return 0.f;
	}

	IPlayerBelongInterface* PlayerBelongEntity = Cast<IPlayerBelongInterface>(DamageCauser);
	if (PlayerBelongEntity == nullptr)
	{
		return 0.f;
	}

	APlayerState* PlayerState = PlayerBelongEntity->GetOwnerPlayer();
	if (PlayerState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null player state in take invade"));
		return 0.f;
	}

	bool ContainsPlayerState = InvaderTeamsInfo.ContainsByPredicate([PlayerState](AInvadeInfo* Info) { return Info->Team == PlayerState; });

	//Reduce other teams points
	int32 OtherTeamsCount = ContainsPlayerState ? InvaderTeamsInfo.Num() - 1 : InvaderTeamsInfo.Num();
	float TeamReducePoints = InvadePoints / OtherTeamsCount;
	
	//Remove ivaders that lost control over building and reduce points
	for (int32 i = InvaderTeamsInfo.Num() - 1; i >= 0; i--)
	{
		AInvadeInfo* Info = InvaderTeamsInfo[i];
		if (Info->Team != PlayerState)
		{
			Info->InvadePoints -= TeamReducePoints;
		}
		if (Info->InvadePoints <= 0.f)
		{
			InvaderTeamsInfo.RemoveAt(i);
		}
	}

	//Add points to invader
	if (!ContainsPlayerState)
	{
		AInvadeInfo* NewInfo = GetWorld()->SpawnActor<AInvadeInfo>();
		NewInfo->Team = PlayerState;
		NewInfo->InvadePoints = 0.f;
		InvaderTeamsInfo.Add(NewInfo);
	}

	AInvadeInfo* const * InvadeInfoData = InvaderTeamsInfo.FindByPredicate([PlayerState](auto Info) { return Info->Team == PlayerState; });
	if (InvadeInfoData == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Concurrent bug prevented in invadable component (array)"));
		return 0.f;
	}

	AInvadeInfo* InvadeInfo = InvadeInfoData[0];
	if (InvadeInfo == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Concurrent bug prevented in invadable component (pointer)"));
		return 0.f;
	}

	InvadeInfo->InvadePoints += InvadePoints;

	//Clamp to max invade points
	if (InvadeInfo->InvadePoints > MaximumInvadePoints)
	{
		InvadeInfo->InvadePoints = MaximumInvadePoints;
	}

	//Check for invasion done
	if (InvadeInfo->InvadePoints == MaximumInvadePoints)
	{
		NotifyInvadeDone(PlayerState);
	}

	return InvadePoints;
}

float UInvadableComponent::GetInvadePointsByPlayer(APlayerState* Player) const
{
	AInvadeInfo* const *InfoData = InvaderTeamsInfo.FindByPredicate([Player](auto Info) { return Info->Team == Player; });
	if (InfoData == nullptr)
	{
		return 0.f;
	}
	AInvadeInfo* const Info = InfoData[0];
	return Info == nullptr ? 0.f : Info->InvadePoints;
}

bool UInvadableComponent::CharacterAlreadyInvadedBuilding(AActor* Character) const
{
	IPlayerBelongInterface* Building = Cast<IPlayerBelongInterface>(GetOwner());
	return !Building->IsEnemyToActor(Character);
}

void UInvadableComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInvadableComponent, InvaderTeamsInfo);
}
