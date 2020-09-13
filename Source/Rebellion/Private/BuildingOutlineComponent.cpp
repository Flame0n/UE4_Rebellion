// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingOutlineComponent.h"

UBuildingOutlineComponent::UBuildingOutlineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuildingOutlineComponent::BeginPlay()
{
	Super::BeginPlay();

	Controller = Cast<APlayerController>(GetOwner());

	PrimaryComponentTick.TickInterval = .05f;
}

void UBuildingOutlineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool HitResult = Controller->GetHitResultUnderCursorByChannel(BuildingCollisionChannel, true, Hit);
	if(HitResult && Hit.bBlockingHit)
	{
		EnableOutline(Hit.GetActor());
	}
	else
	{
		DisableAllOutlines();
	}
}

void UBuildingOutlineComponent::EnableOutline(AActor * Target)
{
	if (!IsValid(Target) || OutlinedBuildings.Contains(Target))
	{
		return;
	}
	DisableAllOutlines();
	OutlinedBuildings.Add(Target);
	UStaticMeshComponent* StaticMesh = Target->FindComponentByClass<UStaticMeshComponent>();
	if (StaticMesh == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Static mesh in outline component is null (enable) %s"), *Target->GetName());
		return;
	}
	StaticMesh->SetRenderCustomDepth(true);
}

void UBuildingOutlineComponent::DisableOutline(AActor * Target)
{
	UStaticMeshComponent* StaticMesh = Target->FindComponentByClass<UStaticMeshComponent>();
	if (StaticMesh == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Static mesh in outline component is null (disable) %s"), *Target->GetName());
		return;
	}
	StaticMesh->SetRenderCustomDepth(false);
}

void UBuildingOutlineComponent::DisableAllOutlines()
{
	if (OutlinedBuildings.Num() == 0)
	{
		return;
	}

	for (AActor* Building : OutlinedBuildings)
	{
		DisableOutline(Building);
	}

	OutlinedBuildings.Empty();
}

void UBuildingOutlineComponent::SetCollisionChannel(ETraceTypeQuery Channel) { 
	BuildingCollisionChannel = Channel; 
}
	


