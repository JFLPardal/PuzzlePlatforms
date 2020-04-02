 // Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);		// for making this actor a replicateable one on the client side
		SetReplicateMovement(true); // inform that the movement on this actor is to be replicated to the client
	}
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);

}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector Location = GetActorLocation();
		float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		float JourneyTraveled = (Location - GlobalStartLocation).Size();
		
		if (JourneyTraveled > JourneyLength)
		{
			FVector tempLocation = GlobalStartLocation;
			GlobalStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = tempLocation;
		}

		FVector DirectionOfTarget = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		Location += DirectionOfTarget * Speed * DeltaTime;
		SetActorLocation(Location);
	}
}
