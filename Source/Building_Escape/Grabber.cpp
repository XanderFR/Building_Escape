// Copyright Alexander Respicio 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//Check for the physics handle component
	FindPhysicsHandle();

	//setup input component
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>(); //gives component a responsiveness to player input
	if (InputComponent) //protection code
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab); //map user input to a function call
		//this refers to the grabber
		//when one of the ["Grab"] buttons is [Pressed], [this] object runs the [Grab function]

		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		//when one of the ["Grab"] buttons is [Release], [this] object runs the [Release function]
	}
}

void UGrabber::FindPhysicsHandle()
{
	//Check for the physics handle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); //Find PhysicsHandle component atached to DefaultPawn_BP
	if (PhysicsHandle == nullptr) //protection code
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	//TODO to only ray cast when key is pressed and see if we reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent(); //represent the thing hit by the green laser
	AActor* ActorHit = HitResult.GetActor();

	//If we hit something then attach the physics handle
	if (ActorHit) //if an actor is hit by the green laser, attach physics handle
	{
		if(!PhysicsHandle) {return;} //if PhysicsHandle is a null pointer; protection code; PhysicsHandle not set
		//TODO attach physics handle
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab, //the thing hit by the green laser
			NAME_None,
			GetPlayerReach()); //the end of the green laser
	}
}

FVector UGrabber::GetPlayerReach() const
{
	//Get player viewpoint
	FVector PlayerViewPointLocation; //player location in game world relative to origin (0, 0, 0)
	FRotator PlayerViewPointRotation; //rotation values, player direction in game world

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	//Draw a line from player showing the reach
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach; //origin -> player -> 100cm from direction player is facing	
}

void UGrabber::Release()
{
	if(!PhysicsHandle) {return;} //if PhysicsHandle is a null pointer; protection code; PhysicsHandle not set
	//TODO remove / release the physics handle
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!PhysicsHandle) {return;} //if PhysicsHandle is a null pointer; protection code; PhysicsHandle not set
	//If the physics handle is attached to an appropriate actor
	if(PhysicsHandle->GrabbedComponent)
	{
		//Move the object we are holding
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	//Ray-cast out to a certain distance (Reach)
	FCollisionQueryParams TraceParams(
		FName(TEXT("")), 
		false, //are you using complex collision
		GetOwner() //what actors to ignore, ignore sphere of player pawn
	);

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit, //store green ray it result
		GetPlayerWorldPosition(), //startpoint of vector
		GetPlayerReach(), //endpoint of vector
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), //are you hitting a physics body object
		TraceParams //ignore green ray hitting the pawn already
	);

	return Hit;
}

FVector UGrabber::GetPlayerWorldPosition() const
{
	//Get player viewpoint
	FVector PlayerViewPointLocation; //player location in game world relative to origin (0, 0, 0)
	FRotator PlayerViewPointRotation; //rotation values, player direction in game world

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}