// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitCharacter.h"
#include "GameplayGameMode.h"
#include "Math/UnrealMathUtility.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "InvadeSpotQueryContext.generated.h"

/**
 * 
 */
UCLASS()
class REBELLION_API UInvadeSpotQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()
	
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

private:

	float RadiusOffset = 100;

	float GetTargetRadius(AActor* Target) const;

	FVector GetPointAroundTarget(FVector TargetLocation, float TargetRadius, int32 PointsCount, int32 PointNumber) const;
};