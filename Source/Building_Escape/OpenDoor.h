// Copyright Alexander Respicio 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();

	//tracks whether sound has been played
	//ensure alternating true and flase values for opening and closing door
	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:
	float InitialYaw;
	float CurrentYaw;

	//UPROPERTIES that can be seen in Unreal Editor in OpenDoor class
	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 50.f;

	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.f;

	UPROPERTY(EditAnywhere) //connects OpenDoor code to PressurePlate
	ATriggerVolume* PressurePlate = nullptr;

	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f; //makes door stay open time available in UE editor and editable

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 1.f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
