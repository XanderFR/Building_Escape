// Copyright Alexander Respicio 2021

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw; //Get door initial yaw value
	CurrentYaw = InitialYaw; //set current yaw equal to current yaw
	TargetYaw += InitialYaw; //create a target yaw that is current yaw + 90 degrees

	FindPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>(); //find the audio component of open door
	if(!AudioComponent) //if audio compnent is a nullptr, not set to anything
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate) //if pressureplate is not set; if OpenDoor / Pressure Plate setting set to none
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the Open Door component on it, but no Pressureplate set!"), *GetOwner()->GetName());
		//ActorName has the open door component on it, but no pressureplate set
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoors) //if pressure plate is set and the actor that opens and pressure plate occupy the same space
	{
		OpenDoor(DeltaTime); //run the open door function
		DoorLastOpened = GetWorld()->GetTimeSeconds(); //set the time the door completely opens to the variable
	}
	else //if pressure plate and default pawn are not overlapping
	{
		//if the door has been open longer than DoorCloseDelay
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay) //if the current world time minus the time the door was fully open is greater than 2 seconds, close the door
		{
			CloseDoor(DeltaTime); //close the door
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * DoorOpenSpeed); //modifies door rotation yaw value so it goes from current yaw to target yaw on a slow pace; point a to point b function; slowly go from yaw 1 to yaw 2
	FRotator DoorRotation = GetOwner()->GetActorRotation(); //variable representing current door degree values; {pitch, yaw, roll}
	DoorRotation.Yaw = CurrentYaw; //modify door yaw value so it goes from current yaw to target yaw slowly; {pitch, yaw(yaw1->yaw1+90 slowly), roll}
	GetOwner()->SetActorRotation(DoorRotation); //code to animate the door; make door follow this sequence of events: {pitch, yaw, roll} -> {pitch, yaw + 90, roll} slowly

	CloseDoorSound = false; //ensure close door sound doesn't play
	if(!AudioComponent){return;}
	//plays the sound
	if (!OpenDoorSound) //if opendoorsound isn't playing at the time opendoor function runs
	{
		AudioComponent->Play(); //play sound
		OpenDoorSound = true; //proof that opendoorsound played on opendoor()
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed); //modifies door rotation yaw value so it goes from current yaw to initial yaw on a slow pace; point b to point a function; slowly go from yaw 2 to yaw 1
	FRotator DoorRotation = GetOwner()->GetActorRotation(); //variable representing current door degree values; {pitch, yaw, roll}
	DoorRotation.Yaw = CurrentYaw; //modify door yaw value so it goes from current yaw to target yaw slowly; {pitch, yaw(yaw1->yaw1-90 slowly), roll}
	GetOwner()->SetActorRotation(DoorRotation); //code to animate the door; make door follow this sequence of events: {pitch, yaw, roll} -> {pitch, yaw - 90, roll} slowly

	OpenDoorSound = false; //ensure open door sound doesn't play
	if(!AudioComponent){return;}
	//plays the sound
	if (!CloseDoorSound) //if closedoorsound isn't playing at the time closedoor function runs
	{
		AudioComponent->Play(); //play sound
		CloseDoorSound = true; //proof that closedoorsound played on closedoor()
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	//Find all overlapping actors
	TArray<AActor*> OverlappingActors; //array for holding actors
	if(!PressurePlate) //if pressureplate is not set; if OpenDoor / Pressure Plate setting set to none; protection code
	{
		return TotalMass;
	}

	PressurePlate->GetOverlappingActors(OUT OverlappingActors); //put actors inside the PressurePlate into the array

	//Add up their masses
	for(AActor* Actor : OverlappingActors) //for loop that trvareses the actors of the array
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass(); //get the mass of every actor in OverlappingActors and add them into TotalMass variable
	}
	return TotalMass;
}