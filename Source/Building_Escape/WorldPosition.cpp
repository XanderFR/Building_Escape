// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	// FString Log = TEXT("Hello!"); //FString variable
	// FString* PtrLog = &Log; //pointer that had the address of Log
	// Log.Len();

	// //access aspects / functions of the final destination of a pointer
	// (*PtrLog).Len(); //final destination of PtrLog and the content length
	// PtrLog->Len(); //travel through PtrLog to the final destination and find length value

	// UE_LOG(LogTemp, Warning, TEXT("%s"), **PtrLog); //print contents of final destination of pointer using its own name
	
	FString ObjectName = GetOwner()->GetName();

	FString ObjectPosition = GetOwner()->GetActorLocation().ToString();
	
	UE_LOG(LogTemp, Warning, TEXT("%s position in world is: %s"), *ObjectName, *ObjectPosition);

}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

