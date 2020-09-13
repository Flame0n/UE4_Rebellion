// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"

#include "PlayerBelongInterface.h"
#include "TeamBelongInterface.h"

#include "TeamComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REBELLION_API UTeamComponent : public UActorComponent, public IPlayerBelongInterface
{
	GENERATED_BODY()

private:

	UPROPERTY(Replicated)
	APlayerState* OwnerPlayer;

public:		

	// Sets default values for this component's properties
	UTeamComponent();

	UFUNCTION(BlueprintCallable)
	virtual void SetOwnerPlayer(APlayerState* PlayerState) override;

	UFUNCTION(BlueprintCallable)
	virtual APlayerState* GetOwnerPlayer() const override;

	virtual bool IsEnemyToActor(AActor* Other) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
