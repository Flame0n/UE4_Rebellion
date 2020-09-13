// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "GameFramework/Controller.h"

#include "ImprovableEntityInterface.h"

#include "TeamInfo.generated.h"

/**
 * 
 */
UCLASS()
class REBELLION_API ATeamInfo : public AInfo, public IImprovableEntityInterface
{
	GENERATED_BODY()

	TSet<IImprovableEntityInterface*> UnitsInTeam;

	/** Default team */
	static const uint8 NO_TEAM = 255;

	/** Index of this team. */
	UPROPERTY(Replicated)
	uint8 TeamIndex = NO_TEAM;

public:

	/** Default constructor */
	ATeamInfo();

	/** Getter for TeamIndex */
	UFUNCTION(BlueprintCallable)
	uint8 GetTeamIndex() const;

	/** Setter for TeamIndex */
	void SetTeamIndex(uint8 Index);

	/** Check if other team is enemy to current */
	bool IsEnemyToTeam(ATeamInfo* Other) const;

	void AddToTeam(IImprovableEntityInterface* Unit) { UnitsInTeam.Add(Unit); }

	void RemoveFromTeam(IImprovableEntityInterface* Unit) { if (UnitsInTeam.Contains(Unit)) UnitsInTeam.Remove(Unit); }

	bool IsInTeam(IImprovableEntityInterface* Unit) { return UnitsInTeam.Contains(Unit); }

	TSet<IImprovableEntityInterface*> GetUnitsInTeam() const { return UnitsInTeam; }
};
