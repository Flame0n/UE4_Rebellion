// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayGameMode.h"
#include "GameplayGameSession.h"

AGameplayGameMode::AGameplayGameMode(const FObjectInitializer & ObjectInitializer) : 
	Super(ObjectInitializer)
{
}

void AGameplayGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ATeamInfo* NewTeam = GetWorld()->SpawnActor<ATeamInfo>();
	NewTeam->SetTeamIndex(NumPlayers);

	AResourcesInfo* PlayerResources = GetWorld()->SpawnActor<AResourcesInfo>();

	AGameplayPlayerState* PlayerState = NewPlayer->GetPlayerState<AGameplayPlayerState>();
	if (PlayerState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong player state on PostLogin"));
		return;
	}

	PlayerState->SetTeamInfo(NewTeam);
	PlayerState->SetResourcesInfo(PlayerResources);
	PlayerState->SetCharacterAsset(CharacterAsset);
}

AActor* AGameplayGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APlayerStart* SpawnPoint = *ActorItr;
		if (!SpawnPoint->PlayerStartTag.IsEqual(TEXT("Taken")))
		{
			SpawnPoint->PlayerStartTag = TEXT("Taken");
			
			AGameplayPlayerState* State = Player->GetPlayerState<AGameplayPlayerState>();
			if (State == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Wrong player state on spawning player"));
				return SpawnPoint;
			}
			State->SetSpawnPoint(SpawnPoint);
			return SpawnPoint;
		}
	}

	UE_LOG(LogTemp, Fatal, TEXT("No spawn points"));
	return nullptr;
}

void AGameplayGameMode::SpawnSquad(AActor* Destination, AController* PlayerController)
{
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null player controller passed to Spawn Squad"));
		return;
	}

	AGameplayPlayerState* GameplayPlayerState = PlayerController->GetPlayerState<AGameplayPlayerState>();
	if (GameplayPlayerState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid player state passed to Spawn Squad"));
		return;
	}

	if (GameplayPlayerState->GetResourcesInfo()->GetGoldAmount() < SQUAD_PRICE)
	{
		UE_LOG(LogTemp, Log, TEXT("Player %s has no enough money to spawn squad"), *GameplayPlayerState->GetName());
		return;
	}

	AActor* SpawnPoint = GameplayPlayerState->GetSpawnPoint();
	if (SpawnPoint == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No spawn point data available"));
		return;
	}

	if (Destination == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Destination is null at spawn squad"));
		return;
	}

	UClass* SpawnClass = GameplayPlayerState->GetCharacterAsset().Get();
	if (SpawnClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn class not set up in game mode"));
		return;
	}

	FActorSpawnParameters Parameters;
	Parameters.bNoFail = true;

	for (uint8 i = 0; i < UNITS_IN_SQUAD; i++)
	{
		FTransform Transform = SpawnPoint->GetTransform();
		FVector Translation = Transform.GetTranslation();
		Translation.X += i * 100;
		Transform.SetTranslation(Translation);

		AUnitCharacter* Character = GetWorld()->SpawnActor<AUnitCharacter>(SpawnClass, Transform, Parameters);
		if (Character == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Character is null after spawning"));
			return;
		}
		Character->SetOwnerPlayer(GameplayPlayerState);
		Character->SetNumberInSquad(i);

		SpawnItemFor(Character, SwordAsset, TEXT("WeaponSocket"));
		SpawnItemFor(Character, TorchAsset, TEXT("ItemSocket"));
		
		AController* Controller = Character->GetController();
		if (Controller == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Character has null controller"));
			return;
		}

		AUnitAIController* UnitController = Cast<AUnitAIController>(Controller);
		if (UnitController == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Character has wrong controller"));
			return;
		}

		UnitController->SetDestination(Destination);
	}

	GameplayPlayerState->GetResourcesInfo()->DecreaseGoldAmountBy(SQUAD_PRICE);
}

void AGameplayGameMode::SpawnItemFor(ACharacter* Character, TSubclassOf<AActor> Asset, FName SocketName)
{
	UClass* ItemClass = Asset.Get();
	if (ItemClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Item asset isn't set in game mode"));
	}

	FTransform SocketTransform = Character->GetMesh()->GetSocketTransform(SocketName);

	AActor* Item = GetWorld()->SpawnActor<AActor>(ItemClass, SocketTransform);
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);

	Item->AttachToComponent(Character->GetMesh(), Rules, SocketName);
	Item->SetActorEnableCollision(false);
}

void AGameplayGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	AController* VirtualController = CreateVirtualPlayer();
	AGameplayPlayerState* VirtualState = VirtualController->GetPlayerState<AGameplayPlayerState>();

	AssignDefaultTeamsToBuildings(VirtualState);

	GetWorldTimerManager().SetTimer(StartGameplayTimerHandle, this, &AGameplayGameMode::StartGameplayTimerTick, 1.0f, true, 0.f);
}

bool AGameplayGameMode::ReadyToEndMatch_Implementation()
{
	return IsPlayerWin;
}

void AGameplayGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	//Maybe add some functionality
}

AController* AGameplayGameMode::CreateVirtualPlayer()
{
	AGameplayComputerAIController* Controller = GetWorld()->SpawnActor<AGameplayComputerAIController>();
	ComputerAIControllers.Add(Controller);

	ATeamInfo* NewTeam = GetWorld()->SpawnActor<ATeamInfo>();
	NewTeam->SetTeamIndex(27);

	AGameplayPlayerState* PlayerState = Controller->GetPlayerState<AGameplayPlayerState>();
	PlayerState->SetTeamInfo(NewTeam);
	PlayerState->SetSpawnPoint(ChoosePlayerStart_Implementation(Controller));
	PlayerState->SetCharacterAsset(PatrolCharacterAsset);

	AResourcesInfo* ResourcesInfo = GetWorld()->SpawnActor<AResourcesInfo>();
	PlayerState->SetResourcesInfo(ResourcesInfo);

	return Controller;
}

void AGameplayGameMode::AssignDefaultTeamsToBuildings(APlayerState* State)
{
	for (TActorIterator<ABuildingActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ABuildingActor* Building = *ActorItr;
		IPlayerBelongInterface* PlayerBelongEntity = Cast<IPlayerBelongInterface>(Building);
		if (PlayerBelongEntity != nullptr)
		{
			PlayerBelongEntity->SetOwnerPlayer(State);
		}
	}
}

void AGameplayGameMode::StartGameplayTimerTick()
{
	if (GameState->GetServerWorldTimeSeconds() > TIME_BEFORE_GAME_STARTS && NumPlayers == NUM_PLAYERS_TO_START_GAME)
	{
		StartGameplay();
		GetWorldTimerManager().ClearTimer(StartGameplayTimerHandle);
	}

	if(GameState->GetServerWorldTimeSeconds() > TIME_BEFORE_GAME_STARTS && NumPlayers != NUM_PLAYERS_TO_START_GAME)
	{
		AGameplayGameSession* Session = Cast<AGameplayGameSession>(GameSession);
		Session->KickAllPlayers();
		GetWorldTimerManager().ClearTimer(StartGameplayTimerHandle);
	}
}

void AGameplayGameMode::StartGameplay()
{
	TArray<APlayerState*> Players = GameState->PlayerArray;
	for (APlayerState* Player : Players)
	{
		AGameplayPlayerState* PlayerState = Cast<AGameplayPlayerState>(Player);
		PlayerState->SetIsGameplayStarted(true);
		AResourcesInfo* ResourcesInfo = GetWorld()->SpawnActor<AResourcesInfo>();
		PlayerState->SetResourcesInfo(ResourcesInfo);
	}

	AddGoldToAllPlayers(INITIAL_GOLD_AMOUNT);
	AddGoldTimerDelegate.BindUFunction(this, FName("AddGoldToAllPlayers"), ADD_GOLD_RATE);
	GetWorldTimerManager().SetTimer(AddGoldTimerHandle, AddGoldTimerDelegate, 1.f, true);

	AGameplayComputerAIController* VirtualController = ComputerAIControllers[0];
	SpawnSquad(VirtualController->GetPlayerState<AGameplayPlayerState>()->GetSpawnPoint(), VirtualController);
}

void AGameplayGameMode::AddGoldToAllPlayers(int32 Amount)
{
	TArray<APlayerState*> Players = GameState->PlayerArray;
	for (APlayerState* Player : Players)
	{
		AGameplayPlayerState* PlayerState = Cast<AGameplayPlayerState>(Player);
		PlayerState->GetResourcesInfo()->IncreaseGoldAmountBy(Amount);
	}
}

void AGameplayGameMode::OnInvadeDone(APlayerState * Player, ABuildingActor* Building)
{
	UClass* CastleClass = CastleClassAsset.Get();
	if (CastleClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Castle asset not set in gamemode"));
		return;
	}

	if (Building->IsA(CastleClass))
	{
		WinGame(Player);
		return;
	}

	const FImprovementData& Data = Building->GetProvidedImprovementData();

	//Kick building from previous player team. Remove bonuses from old player that invades this building
	APlayerState* OldOwner = Building->GetOwnerPlayer();
	ITeamBelongInterface* OldTeamBelongEntity = Cast<ITeamBelongInterface>(OldOwner);
	if (OldTeamBelongEntity != nullptr)
	{
		ATeamInfo* OldPlayerTeam = OldTeamBelongEntity->GetTeamInfo();
		OldPlayerTeam->RemoveFromTeam(Building);
	}

	IImprovableEntityInterface* OldImprovableEntity = Cast<IImprovableEntityInterface>(OldOwner);
	if (OldImprovableEntity != nullptr)
	{
		OldImprovableEntity->DetachImprovementData(Data);
	}

	//Join new player's team. Add bonuses to new player
	ITeamBelongInterface* NewTeamBelongInterface = Cast<ITeamBelongInterface>(Player);
	if (NewTeamBelongInterface != nullptr)
	{
		ATeamInfo* PlayerTeam = NewTeamBelongInterface->GetTeamInfo();
		PlayerTeam->AddToTeam(Building);
	}

	IImprovableEntityInterface* NewImprovableEntity = Cast<IImprovableEntityInterface>(Player);
	if (NewImprovableEntity != nullptr)
	{
		NewImprovableEntity->AppendImprovementData(Data);
	}

	Building->SetOwnerPlayer(Player);
	Building->MulticastClientGotInvade(Player);

	UE_LOG(LogTemp, Log, TEXT("Detaching from %s, Atatching to %s, building: %s"), *OldOwner->GetName(), *Player->GetName(), *Building->GetName());
}

void AGameplayGameMode::WinGame(APlayerState* Winner)
{
	IsPlayerWin = true;

	AGameplayGameSession* Session = Cast<AGameplayGameSession>(GameSession);
	Session->HandleMatchWasEnded(Winner);
}