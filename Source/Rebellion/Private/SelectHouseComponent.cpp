// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectHouseComponent.h"

USelectHouseComponent::USelectHouseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USelectHouseComponent::BeginPlay()
{
	Super::BeginPlay();
}


void USelectHouseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USelectHouseComponent::SelectBuilding(FHitResult hitResult) {
	if (RedArrowAsset == nullptr || BlueArrowAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Arrow asset in select component not defined"));
		return;
	}

	ABuildingActor* PendingBuilding = Cast<ABuildingActor>(hitResult.Actor.Get());
	if (!PendingBuilding) {
		return;
	}

	AGameplayPlayerState* OwnerPlayer = GetOwner()->GetNetOwningPlayer()->GetPlayerController(GetWorld())->GetPlayerState<AGameplayPlayerState>();
	ATeamInfo* TeamInfo = OwnerPlayer->GetTeamInfo();
	bool DoesOwnBase = TeamInfo->IsInTeam(PendingBuilding);

	if (DoesOwnBase)
	{
		if (SelectedBase.Key != nullptr)
		{
			SelectedBase.Value->Destroy();
		}
	}
	else
	{
		if (SelectedTarget.Key != nullptr)
		{
			SelectedTarget.Value->Destroy();
		}
	}

	FVector Origin;
	FVector Extent;
	PendingBuilding->GetActorBounds(true, Origin, Extent);

	FRotator Rotation = PendingBuilding->GetActorRotation();
	Origin.Z += Extent.Z + ArrowHeightOffset;

	if (DoesOwnBase)
	{
		AActor* SpawnedArrow = GetWorld()->SpawnActor<AActor>(BlueArrowAsset, Origin, Rotation);
		SelectedBase.Key = PendingBuilding;
		SelectedBase.Value = SpawnedArrow;
	}
	else
	{
		AActor* SpawnedArrow = GetWorld()->SpawnActor<AActor>(RedArrowAsset, Origin, Rotation);
		SelectedTarget.Key = PendingBuilding;
		SelectedTarget.Value = SpawnedArrow;
	}
}

AActor * USelectHouseComponent::GetNextTarget() const
{
	return SelectedTarget.Key;
}