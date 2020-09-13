// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "GameFramework/PlayerState.h"
#include "InvadeInfo.generated.h"

/**
 * 
 */
UCLASS()
class REBELLION_API AInvadeInfo : public AInfo
{
	GENERATED_BODY()
	
public:

	AInvadeInfo();

	UPROPERTY(Replicated)
	APlayerState* Team;

	UPROPERTY(Replicated)
	float InvadePoints;
};
