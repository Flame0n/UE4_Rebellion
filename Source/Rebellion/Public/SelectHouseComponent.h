// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"

#include "GameplayPlayerState.h"
#include "BuildingActor.h"

#include "SelectHouseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REBELLION_API USelectHouseComponent : public UActorComponent
{
	GENERATED_BODY()

	static constexpr float ArrowHeightOffset = 100.0f;
	
	UClass* RedArrowAsset;
	UClass* BlueArrowAsset;

	TPair<AActor*, AActor*> SelectedBase;
	TPair<AActor*, AActor*> SelectedTarget;

public:	
	// Sets default values for this component's properties
	USelectHouseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SelectBuilding(FHitResult hitResult);

	UFUNCTION(BlueprintCallable)
	AActor* GetNextTarget() const;

	void SetRedArrowAsset(UClass* Asset) { RedArrowAsset = Asset; }

	void SetBlueArrowAsset(UClass* Asset) { BlueArrowAsset = Asset; }
};
