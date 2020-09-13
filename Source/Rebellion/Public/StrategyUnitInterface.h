// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StrategyUnitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStrategyUnitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REBELLION_API IStrategyUnitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual float PerformAttack(AActor* Target) PURE_VIRTUAL(UUndoRedoItemObject, return 0.f;);

	virtual float PerformInvade(AActor* Target) PURE_VIRTUAL(UUndoRedoItemObject, return 0.f;);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead() const;

	virtual void Die() PURE_VIRTUAL(UUndoRedoItemObject, );
};
