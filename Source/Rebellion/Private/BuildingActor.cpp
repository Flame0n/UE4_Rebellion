// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingActor.h"
#include "GameplayGameMode.h"
#include "TeamComponent.h"
#include "InvadableComponent.h"

ABuildingActor::ABuildingActor() : 
	TeamComponent(CreateDefaultSubobject<UTeamComponent>(TEXT("Team Component"))),
	InvadableComponent(CreateDefaultSubobject<UInvadableComponent>(TEXT("Invadable Component")))
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 1.0f;

	TeamComponent->SetNetAddressable();
	TeamComponent->SetIsReplicated(true);

	InvadableComponent->SetNetAddressable();
	InvadableComponent->SetIsReplicated(true);
}

void ABuildingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABuildingActor::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	const float ActualInvade = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (InvadableComponent == nullptr)
	{
		return 0.f;
	}

	return InvadableComponent->TakeInvade(ActualInvade, DamageEvent, EventInstigator, DamageCauser);
}

void ABuildingActor::AppendImprovementData(const FImprovementData & Data)
{
	InvadableComponent->IncreaseResistance(Data.BuildingsResistance);
}

void ABuildingActor::DetachImprovementData(const FImprovementData & Data)
{
	InvadableComponent->DecreaseResistance(Data.BuildingsResistance);
}

APlayerState* ABuildingActor::GetOwnerPlayer() const {
	return TeamComponent->GetOwnerPlayer(); 
}

void ABuildingActor::SetOwnerPlayer(APlayerState* Player) 
{
	TeamComponent->SetOwnerPlayer(Player);
}

bool ABuildingActor::IsEnemyToActor(AActor* Actor) const
{
	return TeamComponent->IsEnemyToActor(Actor);
}

void ABuildingActor::HandleInvadeDone(APlayerState* Player)
{
	AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->OnInvadeDone(Player, this);
}

void ABuildingActor::MulticastClientGotInvade_Implementation(APlayerState* Player)
{
	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (LocalPlayerController == nullptr)
	{
		return;
	}

	if (LocalPlayerController->PlayerState != Player)
	{
		if (IsValid(LocalInvadeFlag))
		{
			LocalInvadeFlag->Destroy();
			LocalInvadeFlag = nullptr;
		}
	}
	else
	{
		UClass* FlagClass = FlagAsset.Get();
		if (FlagClass == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Flag asset not set up in building"));
			return;
		}

		FVector Origin;
		FVector Extent;
		GetActorBounds(true, Origin, Extent);

		FTransform Transform;
		Origin.X += Extent.X;
		Origin.Y += Extent.Y;
		Transform.SetLocation(Origin);

		LocalInvadeFlag = GetWorld()->SpawnActor<AActor>(FlagClass, Transform);
	}
}

const FImprovementData& ABuildingActor::GetProvidedImprovementData() const { 
	return ProvidedImprovementData; 
}