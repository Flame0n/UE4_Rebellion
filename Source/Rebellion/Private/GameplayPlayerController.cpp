// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayPlayerController.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

AGameplayPlayerController::AGameplayPlayerController() : 
	SelectHouseComponent(CreateDefaultSubobject<USelectHouseComponent>(TEXT("Select Building Component"))),
	BuildingOutlineComponent(CreateDefaultSubobject<UBuildingOutlineComponent>(TEXT("Building Outline Component")))
{}

void AGameplayPlayerController::OnMouseClick()
{
	FHitResult Hit;
	if (GetHitResultUnderCursorByChannel(BuildingTraceType, true, Hit))
	{
		if (Hit.bBlockingHit)
		{
			SelectHouseComponent->SelectBuilding(Hit);
		}
	}
}

void AGameplayPlayerController::ClientSpawnSquad()
{
	RPCSpawnSquad(SelectHouseComponent->GetNextTarget(), this);
}

void AGameplayPlayerController::RPCSpawnSquad_Implementation(AActor* Destination, AGameplayPlayerController* Controller)
{
	AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Game mode is null in RPC from Controller"));
		return;
	}
	GameMode->SpawnSquad(Destination, Controller);
}

bool AGameplayPlayerController::RPCSpawnSquad_Validate(AActor* Destination, AGameplayPlayerController* Controller)
{
	return true;
}

void AGameplayPlayerController::NotifyMatchWasEnded(APlayerState * Winner)
{
	UE_LOG(LogTemp, Log, TEXT("Client got notification"))
	bool IsWin = PlayerState == Winner;
	ClientMatchWasEnded(IsWin);
}

void AGameplayPlayerController::ClientMatchWasEnded_Implementation(bool IsWin)
{
	EventMatchWasEnded(IsWin);
}

void AGameplayPlayerController::OnPossess(APawn * aPawn)
{
	Super::OnPossess(aPawn);
}

void AGameplayPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	
	UClass* RedAsset = RedArrowAsset.Get();
	UClass* BlueAsset = BlueArrowAsset.Get();
	if (RedAsset == nullptr || BlueAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Arrow asset not configured in gameplay player controller"));
	}
	else
	{
		SelectHouseComponent->SetRedArrowAsset(RedAsset);
		SelectHouseComponent->SetBlueArrowAsset(BlueAsset);
	}

	if (InputComponent != nullptr)
	{
		InputComponent->BindAction(TEXT("SetDestination"), IE_Pressed, this, &AGameplayPlayerController::OnMouseClick);
	}

	BuildingTraceType = UEngineTypes::ConvertToTraceType(BuildingCollisionAsset.GetValue());
	BuildingOutlineComponent->SetCollisionChannel(BuildingTraceType);
}