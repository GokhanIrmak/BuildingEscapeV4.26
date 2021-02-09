// Copyright GokhanIrmak 2021

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

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
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);
	// FMath::Lerp(/*CurrentYaw,TargetYaw,0-1*/);
	FRotator ActorRotation = GetOwner()->GetActorRotation();
	// ActorRotation.Yaw = FMath::Lerp(ActorRotation.Yaw,TargetYaw,0.01f);
	//45 is angles-> meaning we will open door in 2 seconds because it will move 45 degrees in one second
	// ActorRotation.Yaw = FMath::FInterpConstantTo(ActorRotation.Yaw,TargetYaw,DeltaTime,45);
	ActorRotation.Yaw = FMath::FInterpTo(ActorRotation.Yaw, TargetYaw, DeltaTime, 2);

	GetOwner()->SetActorRotation(ActorRotation);
	// ...
}
