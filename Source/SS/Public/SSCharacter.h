// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "SSInventoryContainerBase.h"
#include "SSItem.h"
#include "SSConstants.h"

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


UCLASS()
class ASSCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Pawn mesh: 1st person view */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	TSubobjectPtr<class USkeletalMeshComponent> Mesh1P;


	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class UCameraComponent> CameraComponent;



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

	UPROPERTY(VisibleDefaultsOnly, Category=Inventory, Replicated)
		ASSInventoryContainerBase* PlayerClothes;

	UPROPERTY(VisibleDefaultsOnly, Category=Inventory, Replicated)
		ASSInventoryContainerBase* PlayerBelt;

	UPROPERTY(VisibleDefaultsOnly, Category=Inventory, Replicated)
		ASSInventoryContainerBase* PlayerPack;

protected:




	//////////////////////////////////////////////////////
	//  Player Movement

private:

	const float MAX_WALK_SPEED = 3.0f;
	const float CROUCH_SPEED_MULTIPLIER = 0.5f;
	const float JUMP_VELOCITY = 4.2f;

protected:

	///////////////////////////////////////////////////////////
	// Player Movement

	void MoveForward(float Val);

	void MoveRight(float Val);

	void SprintStart();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSprintStart();

	void SprintEnd();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSprintEnd();

	UPROPERTY(Replicated)
	bool bIsCrouching;
	UPROPERTY(Replicated)
	bool bIsPlayerCrouched;
	float CachedCapsuleHalfHeight;
	float CrouchInterpValue;

	void CrouchStart();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCrouchStart();

	void CrouchEnd();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCrouchEnd();

	void CrouchImplementation(float DelaSeconds);

private:

		float SprintMultiplier;

		float CachedDefaultWalkSpeed;


	///////////////////////////////////////////////////////////
	// Player Interaction
protected:

	void TraceForInteraction();

	void Interact();

	// [Server] Check with server to see if we can interact with object
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteract();

	// [Client] Server to client to give go/no-go on interaction
	UFUNCTION(Client, Reliable, WithValidation)
	void ServerInteractResponse(bool bInteract);


private:
	UPROPERTY(Replicated)
	ASSItem* CurrentInteactableItem;

	const float INTERACTION_RANGE = 200.0f;



	///////////////////////////////////////////////////
	//  Player Input Management

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	
	///////////////////////////////////////////////////////
	//  Accessor/Mutator

public:

	FVitalsStruct GetPlayerVitals();




};

