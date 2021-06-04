// Copyright GokhanIrmak 2021

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	OpenAngle += InitialYaw;
	CurrentYaw = InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component bu no PressurePlate set!!"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	
	if (TotalMassOfActors() > RequiredMass)
	{
		OpenDoor(DeltaTime);
		//DoorLast Opened = When the door was opened
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if ((GetWorld()->GetTimeSeconds() - DoorLastOpened) > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
	// if the door has been open longer than DoorCloseDelay

	// FMath::Lerp(/*CurrentYaw,TargetYaw,0-1*/);
	// DoorRotation.Yaw = FMath::Lerp(DoorRotation.Yaw,TargetYaw,0.01f);
	//45 is angles-> meaning we will open door in 2 seconds because it will move 45 degrees in one second
	// DoorRotation.Yaw = FMath::FInterpConstantTo(DoorRotation.Yaw,TargetYaw,DeltaTime,45);
	// ...
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	//Find All overlapping Actors
	TArray<AActor *> OverlappingActors;
	
	if (!PressurePlate)
	{
		//Safe Pointer
		UE_LOG(LogTemp, Warning, TEXT("PressurePlate cannot be found for the: %s!"), *(GetOwner()->GetName()));
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Add up Their Masses
	for (AActor *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}