// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "ResourcesInfo.generated.h"

/**
 * 
 */
UCLASS()
class REBELLION_API AResourcesInfo : public AInfo
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	int32 GoldAmount = 0;

public:

	/** Default constructor */
	AResourcesInfo();

	UFUNCTION(BlueprintCallable)
	int32 GetGoldAmount() const;

	void IncreaseGoldAmountBy(int32 Value);

	void DecreaseGoldAmountBy(int32 Value);
	
};
