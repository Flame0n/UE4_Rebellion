// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TeamInfo.h"
#include "TeamBelongInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTeamBelongInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REBELLION_API ITeamBelongInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual ATeamInfo* GetTeamInfo() const PURE_VIRTUAL(UUndoRedoItemObject, return nullptr;);

	virtual bool IsEnemyToPlayer(APlayerState* Player) const PURE_VIRTUAL(UUndoRedoItemObject, return false;);

};
