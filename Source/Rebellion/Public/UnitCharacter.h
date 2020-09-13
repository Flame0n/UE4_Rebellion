// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Perception/AISense_Sight.h"
#include "Components/WidgetComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

#include "HealthWidgetBase.h"
#include "StrategyUnitInterface.h"
#include "PlayerBelongInterface.h"
#include "UnitAnimationBlueprint.h"
#include "ImprovableEntityInterface.h"

#include "UnitCharacter.generated.h"

class UHealthComponent;
class UAttackComponent;
class UTeamComponent;
class UInvadeComponent;

UCLASS()
class REBELLION_API AUnitCharacter : public ACharacter, public IImprovableEntityInterface, public IStrategyUnitInterface, public IPlayerBelongInterface
{
	GENERATED_BODY()

	/** Character components*/
	UHealthWidgetBase* HealthWidget;
	UAIPerceptionStimuliSourceComponent* StimuliSource;
	UHealthComponent* HealthComponent;
	UAttackComponent* AttackComponent;
	UTeamComponent* TeamComponent;
	UInvadeComponent* InvadeComponent;

	/** Position in squad. Used to determine invade point around the building */
	uint8 NumberInSquad;

protected:

	UFUNCTION(BlueprintCallable)
	void SetHealthWidget(UHealthWidgetBase* Widget);

public:

	/** Asset of collision category */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Assets")
	TEnumAsByte<ECollisionChannel> CharacterCollisionAsset;

	/** Type of the damage caused by this attack. */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	TSubclassOf<class UDamageType> InvadeDamageType;

	/** Type of the damage caused by this attack. */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	TSubclassOf<class UDamageType> MeleeDamageType;

	/** Sets default values for this character's properties */
	AUnitCharacter();

	/** Checks health component */
	virtual bool IsDead_Implementation() const override;

	/** Sets owner player for this character */
	virtual void SetOwnerPlayer(APlayerState* State) override;

	virtual APlayerState* GetOwnerPlayer() const override;
	
	/** Called when character hp is negative after attack */
	virtual void Die() override;

	/** Called when character should die */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void MulticastDie();

	/** Called when character receive damage */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void SetNumberInSquad(uint8 Number) { NumberInSquad = Number; }

	uint8 GetNumberInSquad() const { return NumberInSquad; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void MulticastSetIsAttacking(bool Attacking);

	virtual void AppendImprovementData(const FImprovementData& Data) override;

	virtual void DetachImprovementData(const FImprovementData& Data) override;

	virtual float PerformAttack(AActor* Target) override;

	virtual float PerformInvade(AActor* Target) override;

	virtual bool IsEnemyToActor(AActor* Actor) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
