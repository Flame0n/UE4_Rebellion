// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ImprovableEntityInterface.generated.h"

USTRUCT(BlueprintType)
struct FImprovementData
{
	GENERATED_BODY()

	/** How much damage will be added to units attack */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float UnitsDamage = 0.f;

	/** Armor increase ability to resist attacks */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float UnitsArmor = 0.f;

	/** Invade power defines speed of building invasion */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float UnitsInvadePower = 0.f;

	/** How long the building can stay under pressure */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float BuildingsResistance = 0.f;

	/** One shot of gold that would be given to the player */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float InstantGold = 0.f;

	/** Persistent gold multiplier to gold amount on server tick */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float GoldMultiplier = 0.f;

	FImprovementData& operator+=(const FImprovementData& rhs)
	{
		UnitsDamage += rhs.UnitsDamage;
		UnitsArmor += rhs.UnitsArmor;
		UnitsInvadePower += rhs.UnitsInvadePower;
		BuildingsResistance += rhs.BuildingsResistance;
		GoldMultiplier += rhs.GoldMultiplier;
		return *this;
	}

	FImprovementData& operator-=(const FImprovementData& rhs)
	{
		UnitsDamage -= rhs.UnitsDamage;
		UnitsArmor -= rhs.UnitsArmor;
		UnitsInvadePower -= rhs.UnitsInvadePower;
		BuildingsResistance -= rhs.BuildingsResistance;
		GoldMultiplier -= rhs.GoldMultiplier;
		return *this;
	}
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UImprovableEntityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REBELLION_API IImprovableEntityInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void AppendImprovementData(const FImprovementData& Data) PURE_VIRTUAL(ASpawnVolume::CanRedo, );
	 
	virtual void DetachImprovementData(const FImprovementData& Data) PURE_VIRTUAL(ASpawnVolume::CanRedo, );
};
