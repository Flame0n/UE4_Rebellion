// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAIController.h"

AUnitAIController::AUnitAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PrimaryActorTick.bCanEverTick = false;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Sight Perception Component"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LooseSightRadius;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComponent->ConfigureSense(*SightConfig);

	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AUnitAIController::SenseUpdated);
}

void AUnitAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnitAIController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	ControlledUnit = Cast<IStrategyUnitInterface>(NewPawn);
	if (ControlledUnit == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Not conform interface"), *GetPawn()->GetName());
	}

	UBlackboardComponent* BlackboardComponent;
	if (!UseBlackboard(CharacterBlackboardAsset, BlackboardComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Cant use new blackboard"), *GetPawn()->GetName());
	}

	RunBehaviorTree(CharacterBehaviorTreeAsset);
}

bool AUnitAIController::UpdateEnemy(AActor* Target)
{
	if (Target->GetClass()->ImplementsInterface(UStrategyUnitInterface::StaticClass()) && IStrategyUnitInterface::Execute_IsDead(Target))
	{
		for (AActor* SensedEnemy : SensedEnemies)
		{
			if (!IStrategyUnitInterface::Execute_IsDead(SensedEnemy))
			{
				Blackboard->SetValueAsObject(TEXT("Enemy"), SensedEnemy);
				return true;
			}
		}
		SensedEnemies.Empty();
		Blackboard->ClearValue(TEXT("Enemy"));
		return true;
	}
	else
	{
		return false;
	}
}

void AUnitAIController::SenseUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Stimulus.WasSuccessfullySensed())
	{
		return;
	}

	IPlayerBelongInterface* MyPawn = Cast<IPlayerBelongInterface>(GetPawn());
	if (MyPawn != nullptr && MyPawn->IsEnemyToActor(Actor))
	{
		OnEnemySeen(Actor);
	}
}

void AUnitAIController::OnEnemySeen(AActor* Enemy)
{
	if (!Enemy->GetClass()->ImplementsInterface(UStrategyUnitInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy does not implement UKillableUnit interface"));
		return;
	}

	SensedEnemies.Add(Enemy);

	if (Blackboard->GetValueAsObject(TEXT("Enemy")) == nullptr)
	{
		Blackboard->SetValueAsObject(TEXT("Enemy"), Enemy);
	}	
}

void AUnitAIController::PerformAttack()
{
	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(TEXT("Enemy")));
	if (Target == nullptr)
	{
		return;
	}

	bool WasTargetUpdated = UpdateEnemy(Target);

	if (WasTargetUpdated)
	{
		UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
		if (BehaviorTreeComponent != nullptr)
		{
			BehaviorTreeComponent->RestartTree();
		}
		return;
	}

	ControlledUnit->PerformAttack(Cast<AActor>(Target));
	UpdateEnemy(Target);
}

float AUnitAIController::PerformInvade()
{
	AActor* Destination = Cast<AActor>(Blackboard->GetValueAsObject(TEXT("Destination")));
	if (Destination == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null destination on perform invade"));
		return 0.f;
	}

	return ControlledUnit->PerformInvade(Destination);
}

void AUnitAIController::SetDestination(AActor* NewDestination)
{
	if (NewDestination == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null destination passed to unit AI"));
		return;
	}

	if (Blackboard == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null blackboard at unit AI"));
		return;
	}

	Blackboard->SetValueAsObject(TEXT("Destination"), NewDestination);
}
