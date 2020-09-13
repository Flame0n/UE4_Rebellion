// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayGameSession.h"

void AGameplayGameSession::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ConnectedPlayers.Add(NewPlayer);
}

void AGameplayGameSession::KickAllPlayers()
{
	for (APlayerController* Player : ConnectedPlayers)
	{
		UE_LOG(LogTemp, Log, TEXT("Player %s was kicked"), *Player->GetName());
		KickPlayer(Player, FText::FromString("Match ended"));
	}
}

void AGameplayGameSession::HandleMatchWasEnded(APlayerState* Winner)
{
	for (APlayerController* Player : ConnectedPlayers)
	{
		IMatchStateNotifiable* Notifiable = Cast<IMatchStateNotifiable>(Player);
		if (Notifiable != nullptr)
		{
			Notifiable->NotifyMatchWasEnded(Winner);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant notify player about match end in GameSession"));
		}
	}
}