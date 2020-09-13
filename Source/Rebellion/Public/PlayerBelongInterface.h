// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/PlayerState.h"
#include "PlayerBelongInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerBelongInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REBELLION_API IPlayerBelongInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetOwnerPlayer(APlayerState* State) PURE_VIRTUAL(UUndoRedoItemObject, );

	virtual APlayerState* GetOwnerPlayer() const PURE_VIRTUAL(UUndoRedoItemObject, return nullptr;);

	virtual bool IsEnemyToActor(AActor* Actor) const PURE_VIRTUAL(UUndoRedoItemObject, return false;);

};
