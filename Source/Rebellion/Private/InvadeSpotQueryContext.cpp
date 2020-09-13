// Fill out your copyright notice in the Description page of Project Settings.


#include "InvadeSpotQueryContext.h"

void UInvadeSpotQueryContext::ProvideContext(FEnvQueryInstance & QueryInstance, FEnvQueryContextData & ContextData) const
{
	AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Game mode is null in query context"));
		return;
	}

	UObject* QuerierObject = QueryInstance.Owner.Get();
	if (QuerierObject == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Querier character is null in query context"));
		return;
	}

	AUnitCharacter* QuerierCharacter = Cast<AUnitCharacter>(QuerierObject);
	if (QuerierCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Querier character is invalid in query context"));
		return;
	}

	UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QuerierCharacter);
	AActor* Destination = Cast<AActor>(Blackboard->GetValueAsObject(TEXT("Destination")));
	if (Destination == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Querier character has invalid destination in query context"));
		return;
	}

	FVector ResultingLocation = GetPointAroundTarget(
		Destination->GetTransform().GetLocation(),
		GetTargetRadius(Destination) + RadiusOffset,
		AGameplayGameMode::UNITS_IN_SQUAD,
		QuerierCharacter->GetNumberInSquad()
	);

	UEnvQueryItemType_Point::SetContextHelper(ContextData, ResultingLocation);
}

float UInvadeSpotQueryContext::GetTargetRadius(AActor* Target) const
{
	TArray<UStaticMeshComponent*> Components;
	Target->GetComponents<UStaticMeshComponent>(Components);
	UStaticMesh* Mesh = nullptr;
	for (UStaticMeshComponent* Component : Components)
	{
		Mesh = Component->GetStaticMesh();
	}

	if (Mesh == nullptr)
	{
		return 0;
	}
	
	FVector Size = Mesh->GetBoundingBox().GetSize();

	return FMath::Sqrt(FMath::Pow((Size.X / 2), 2) + FMath::Pow((Size.Y / 2), 2));
}

FVector UInvadeSpotQueryContext::GetPointAroundTarget(FVector TargetLocation, float TargetRadius, int32 PointsCount, int32 PointNumber) const
{
	const float AngleSector = 360.f / PointsCount;
	const float Deviation = AngleSector / 3;
	const float Angle = PointNumber * AngleSector + FMath::RandRange(-Deviation, Deviation);
	const float AngleRadians = Angle * PI / 180.f;

	float Sin, Cos;
	FMath::SinCos(&Sin, &Cos, AngleRadians);
	return FVector(TargetLocation.X + TargetRadius * Cos, TargetLocation.Y + TargetRadius * Sin, TargetLocation.Z);
}