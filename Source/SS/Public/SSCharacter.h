// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "SSItem.h"

#include "GameFramework/Character.h"
#include "SSCharacter.generated.h"


//  Struct to hold all player health realted information
USTRUCT()
struct FVitalsStruct {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Health;

	// Adjust Health
	void AdjustHealth(float Value){
		Health = FMath::Clamp(Health + Value, 0.0f, 100.0f);
	}

	UPROPERTY()
	float Stamina;

	// Adjust Stamina
	void AdjustStamina(float Value){
		Stamina = FMath::Clamp(Stamina + Value, 0.0f, 100.0f);
	}

	UPROPERTY()
	float Hunger;

	// Adjust Hunger
	void AdjustHunger(float Value){
		Hunger = FMath::Clamp(Hunger + Value, 0.0f, 100.0f);
	}

	UPROPERTY()
	float Thirst;

	// Adjust Thirst
	void AdjustThirst(float Value){
		Thirst = FMath::Clamp(Thirst + Value, 0.0f, 100.0f);
	}

	UPROPERTY()
	float Infection;

	// Adjust Infection
	void AdjustInfection(float Value){
		Infection = FMath::Clamp(Infection + Value, 0.0f, 100.0f);
	}

	UPROPERTY()
	float Temperature;

	// Adjust Temperature
	void AdjustTemperature(float Value){
		Temperature = FMath::Clamp(Temperature + Value, 0.0f, 120.0f);
	}

	// constructor
	FVitalsStruct(){
		Health = 100.0f;
		Stamina = 100.0f;
		Hunger = 100.0f;
		Thirst = 100.0f;
		Infection = 0.0f;
		Temperature = 98.6f;
	}
};

USTRUCT()
struct FInventorySlotStruct{

	GENERATED_USTRUCT_BODY()

	FString ItemName;

	UTexture2D* ItemUIImage;

	int32 MaxStackSize;

	int32 CurrentStackSize;

	TArray<ASSItem*> Items;

	bool IsEmpty(){
		return CurrentStackSize <= 0 && MaxStackSize <= 0;
	}

	bool IsFull(){
		return CurrentStackSize == MaxStackSize;
	}


	FInventorySlotStruct(){
		ItemName = "NULL";
		ItemUIImage = NULL;
		MaxStackSize = -1;
		CurrentStackSize = -1;
		Items.Empty();
	}

};

UCLASS(config=Game)
class ASSCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Pawn mesh: 1st person view */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	TSubobjectPtr<class USkeletalMeshComponent> Mesh1P;


	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class UCameraComponent> FirstPersonCameraComponent;



	//////////////////////////////////////////////////////
	//  Player Initialization

	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

private:

	UFUNCTION()
	void InitializePlayer();
		
	///////////////////////////////////////////////////////
	//  Player Vitals

protected:

	UPROPERTY(Replicated)
	FVitalsStruct PlayerVitals;



	//////////////////////////////////////////////////////
	//  Player Inventory

private:

	UPROPERTY(Replicated)
		TArray<FInventorySlotStruct> PlayerInventory;

protected:




	//////////////////////////////////////////////////////
	//  Player Movement

protected:

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	// Handles sprint start
	void SprintStart();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSprintStart();

	// Handles sprint end
	void SprintEnd();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSprintEnd();

	UPROPERTY(Replicated)
	bool bIsCrouching;
	UPROPERTY(Replicated)
	bool bIsPlayerCrouched;
	float CachedCapsuleHalfHeight;
	float CrouchInterpValue;

	// handle crouch start
	void CrouchStart();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCrouchStart();

	// handle crouch end
	void CrouchEnd();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCrouchEnd();

	void CrouchImplementation(float DelaSeconds);



private:

	float SprintMultiplier;

	float CachedDefaultWalkSpeed;



	///////////////////////////////////////////////////
	//  Player Input Management

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	
	///////////////////////////////////////////////////////
	//  Accessor/Mutator

public:

	FVitalsStruct GetPlayerVitals();




};

