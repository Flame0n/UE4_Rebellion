// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"
#include "BuildingOutlineComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REBELLION_API UBuildingOutlineComponent : public UActorComponent
{
	GENERATED_BODY()

	/** Cached owner controller */
	APlayerController* Controller;

	/** COllision channel for future usage. For now it doesn't work */
	ETraceTypeQuery BuildingCollisionChannel;

	/** Use the set instead of one Actor to prevent outline bugs */
	TSet<AActor*> OutlinedBuildings;

	/** Persistent hit variable in order to not create each Tick */
	FHitResult Hit;

	/** Adds outline effect to cpecific actor */
	void EnableOutline(AActor* Target);

	/** Removes outline effect from cpecific actor */
	void DisableOutline(AActor* Target);

	/** Removes all outlines from OutlinedBuildings and clear set */
	void DisableAllOutlines();

public:	
	/** Sets default values for this component's properties */
	UBuildingOutlineComponent();

protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;

public:	
	/** Called every frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCollisionChannel(ETraceTypeQuery Channel);
};
