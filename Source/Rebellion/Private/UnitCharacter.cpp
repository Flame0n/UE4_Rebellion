// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitCharacter.h"

#include "HealthComponent.h"
#include "AttackComponent.h"
#include "TeamComponent.h"
#include "InvadeComponent.h"

AUnitCharacter::AUnitCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimuli Source"));
	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack Component"));
	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("Team Component"));
	InvadeComponent = CreateDefaultSubobject<UInvadeComponent>(TEXT("Invade Component"));

	AttackComponent->SetDamageType(MeleeDamageType);

	TeamComponent->SetNetAddressable();
	TeamComponent->SetIsReplicated(true);

	HealthComponent->SetNetAddressable();
	HealthComponent->SetIsReplicated(true);
}

void AUnitCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (InvadeDamageType.Get() == nullptr || MeleeDamageType.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Damage types not set up correctly in unit character"));
	}
}

void AUnitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnitCharacter::SetHealthWidget(UHealthWidgetBase* Widget) 
{
	HealthWidget = Widget;
}

void AUnitCharacter::MulticastSetIsAttacking_Implementation(bool Attacking)
{
	UUnitAnimationBlueprint* AnimationBlueprint = Cast<UUnitAnimationBlueprint>(GetMesh()->GetAnimInstance());
	AnimationBlueprint->SetIsAttacking(Attacking);
}

void AUnitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUnitCharacter::AppendImprovementData(const FImprovementData& Data)
{
	AttackComponent->IncreaseDamage(Data.UnitsDamage);
	InvadeComponent->IncreaseInvadePower(Data.UnitsInvadePower);
	HealthComponent->IncreaseArmor(Data.UnitsArmor);
}

void AUnitCharacter::DetachImprovementData(const FImprovementData& Data)
{
	AttackComponent->DecreaseDamage(Data.UnitsDamage);
	InvadeComponent->DecreaseInvadePower(Data.UnitsInvadePower);
	HealthComponent->DecreaseArmor(Data.UnitsArmor);
}

bool AUnitCharacter::IsDead_Implementation() const 
{
	return HealthComponent->IsDead();
}

void AUnitCharacter::SetOwnerPlayer(APlayerState* State)
{
	if (TeamComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Character has no Team Component on spawn"));
		return;
	}

	TeamComponent->SetOwnerPlayer(State);

	ITeamBelongInterface* TeamBelongEntity = Cast<ITeamBelongInterface>(State);
	if (TeamBelongEntity != nullptr)
	{
		TeamBelongEntity->GetTeamInfo()->AddToTeam(this);
	}
}

APlayerState* AUnitCharacter::GetOwnerPlayer() const
{
	return TeamComponent->GetOwnerPlayer();
}

float AUnitCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HealthComponent == nullptr)
	{
		return 0.f;
	}

	return HealthComponent->TakeDamage(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
}

void AUnitCharacter::Die()
{
	UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	Blackboard->SetValueAsBool(FName("IsDead"), true);

	//Stop stimuli source
	StimuliSource->UnregisterComponent();

	MulticastDie();
}

void AUnitCharacter::MulticastDie_Implementation()
{
	//Disable health widget
	if (HealthWidget != nullptr)
	{
		HealthWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	//Stop all movement
	GetCharacterMovement()->DisableMovement();

	//Enable ragdoll simuation and disable collisions with other actors
	USkeletalMeshComponent* SelfMesh = GetMesh();
	SelfMesh->SetAllBodiesSimulatePhysics(true);
	SelfMesh->SetCollisionResponseToChannel(CharacterCollisionAsset.GetValue(), ECollisionResponse::ECR_Ignore);

	//Stop animation blueprint
	UUnitAnimationBlueprint* AnimationBlueprint = Cast<UUnitAnimationBlueprint>(GetMesh()->GetAnimInstance());
	AnimationBlueprint->SetIsDead(true);
}

float AUnitCharacter::PerformAttack(AActor* Target)
{
	return AttackComponent->PerformAttack(Target);
}

float AUnitCharacter::PerformInvade(AActor* Target)
{
	return InvadeComponent->PerformInvade(Target);
}

bool AUnitCharacter::IsEnemyToActor(AActor* Actor) const
{
	return TeamComponent->IsEnemyToActor(Actor);
}