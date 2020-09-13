// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Engine.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"

#include "StrategyUnitInterface.h"
#include "PlayerBelongInterface.h"

#include "UnitAIController.generated.h"

/**
 * 
 */
UCLASS()
class REBELLION_API AUnitAIController : public AAIController
{
	GENERATED_BODY()

private:

	const float PeripheralVisionAngleDegrees = 360;
	const float SightRadius = 800;
	const float LooseSightRadius = 1000;
	const float HearingRange = 1000;

	TSet<AActor*> SensedEnemies;

	UAISenseConfig_Sight* SightConfig;

	IStrategyUnitInterface* ControlledUnit;

	bool UpdateEnemy(AActor* Target);
	
public:

	/** Constructor that adds crowd logic */
	AUnitAIController(const FObjectInitializer& ObjectInitializer);

	/** Should be called on spawn */
	void SetDestination(AActor* NewDestination);

	/** Behavior tree to use for driving the character AI. */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior")
	UBehaviorTree* CharacterBehaviorTreeAsset;

	/** Blackboard to use for holding all data relevant to the character AI. */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior")
	UBlackboardData* CharacterBlackboardAsset;

	/** Called from ABP to perform attack */
	UFUNCTION(BlueprintCallable)
	void PerformAttack();

	/** Called from behavior tree to perform invade */
	UFUNCTION(BlueprintCallable)
	float PerformInvade();

	/** Called when controller poassesses pawn */
	virtual void OnPossess(APawn* NewPawn) override;

	/** Called every tick */
	virtual void Tick(float DeltaTime) override;


protected:

	/* Called when AI saw enemy init */
	void OnEnemySeen(AActor* Enemy);

	/* Called when saw another actor */
	UFUNCTION()
	void SenseUpdated(AActor* Actor, FAIStimulus Stimulus);

};
