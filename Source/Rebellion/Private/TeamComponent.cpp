// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamComponent.h"
#include "UnrealNetwork.h" // Should be here

UTeamComponent::UTeamComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTeamComponent::SetOwnerPlayer(APlayerState * PlayerState)
{
	if (PlayerState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Team component got null new owner"));
		return;
	}
	OwnerPlayer = PlayerState;
}

APlayerState * UTeamComponent::GetOwnerPlayer() const
{
	return OwnerPlayer;
}

bool UTeamComponent::IsEnemyToActor(AActor * Other) const
{
	if (Other == nullptr)
	{
		return false;
	}

	ITeamBelongInterface* MyTeamBelongEntity = Cast<ITeamBelongInterface>(OwnerPlayer);
	if (MyTeamBelongEntity == nullptr)
	{
		return false;
	}

	IPlayerBelongInterface* OtherPlayerBelongEntity = Cast<IPlayerBelongInterface>(Other);
	if (OtherPlayerBelongEntity == nullptr)
	{
		return false;
	}
	APlayerState* OtherPlayer = OtherPlayerBelongEntity->GetOwnerPlayer();

	return MyTeamBelongEntity->IsEnemyToPlayer(OtherPlayer);
}

void UTeamComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTeamComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTeamComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UTeamComponent, OwnerPlayer, COND_InitialOnly);
}