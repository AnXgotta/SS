// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "SSInventoryContainerWearable.h"
#include "SSInventoryContainerBase.h"
#include "SSItem.h"
#include "SSConstants.h"
#include "SSInteractable.h"

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


//Controller->IsLocalPlayerController();

UCLASS()
class ASSCharacter : public ACharacter, public ISSInteractable
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class UCameraComponent> CameraComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;


	//////////////////////////////////////////////////////
	//  Player Initialization

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:

	UFUNCTION()
	void InitializePlayer();

	UFUNCTION()
		void InitializeInventory();

		
	///////////////////////////////////////////////////////
	//  Player Vitals

protected:

	UPROPERTY(Replicated)
	FVitalsStruct PlayerVitals;



	//////////////////////////////////////////////////////
	//  Player Inventory


protected:

	// socket name to attach inventory containers to player
	const FName SOCKET_CLOTHES = FName(TEXT("SOCKET_CLOTHES"));
	const FName SOCKET_VEST = FName(TEXT("SOCKET_VEST"));

	// Blueprint to assing in Character defaults for clothes inventory item (for player spawn with clothes)
	UPROPERTY(EditAnywhere, Category=Inventory)
		TSubclassOf<class ASSInventoryContainerWearable> PlayerClothesBlueprint;
	
	// pointer to clothes inventory item
	UPROPERTY(BlueprintReadOnly, Category = Inventory, Replicated)
		ASSInventoryContainerWearable* PlayerClothes;
	
	// Blueprint to assing in Character defaults for belt inventory item (for player spawn with belt)
	UPROPERTY(EditAnywhere, Category = Inventory)
		TSubclassOf<class ASSInventoryContainerWearable> PlayerBeltBlueprint;

	// pointer to belt inventory item
	UPROPERTY(BlueprintReadOnly, Category = Inventory, Replicated)
		ASSInventoryContainerWearable* PlayerBelt;
	
	// Blueprint to assing in Character defaults for pack inventory item (for player spawn with pack)
	UPROPERTY(EditAnywhere, Category = Inventory)
		TSubclassOf<class ASSInventoryContainerWearable> PlayerPackBlueprint;

	// pointer to pack inventory item
	UPROPERTY(BlueprintReadOnly, Category = Inventory, Replicated)
		ASSInventoryContainerWearable* PlayerPack;
	
	// [Server] try to add item to inventory, sends response to client
	UFUNCTION()
	bool AddItemToInventory(ASSItem* ItemToAdd);
	
	// [Client]  response from server when item added to inventory
	UFUNCTION(Client, Reliable, WithValidation)
		void ClientAddItemToInventoryResponse(ASSItem* ItemToAdd);





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

	void ManagePitch(float Val);

private:

		float SprintMultiplier;

		float CachedDefaultWalkSpeed;

		// No rotation = 0, look down (camera rotates up) = 360 - 300, look up (camera rotates down) = 0 - 60
		const float PITCH_UPPER_LIMIT = 60.0f;
		const float PITCH_LOWER_LIMIT = 300.0f;


	///////////////////////////////////////////////////////////
	// Player Interaction
protected:
	// [Client only] trace from players head to recognize object with which to interact
	UFUNCTION()
	void TraceForObjectRecognition();
	// [Server + Client] called on server from client when client wants to interact, server does the things
	UFUNCTION()
	void TraceForObjectInteraction();
	// [Client only] method called on Interact button press
	UFUNCTION()
	void Interact();
	// [Server] handles interaction with object
	UFUNCTION()
		void HandleInteraction();

	// [Server] Check with server to interact with object
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteract();

	// [Client] Server to client to result of interaction
	UFUNCTION(Client, Reliable, WithValidation)
	void ServerInteractResponse(bool bInteract);


private:
	// actor pointer used when interacting
	UPROPERTY()
		AActor* CurrentRecognizedInteractableObject;

	const float INTERACTION_RANGE = 500.0f;



	///////////////////////////////////////////////////
	//  Player Input Management

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	///////////////////////////////////////////////////////
	//  Interactable Interface

	virtual void OnRecognized();

	virtual void OnNotRecognized();

	virtual void OnInteract();
	
	///////////////////////////////////////////////////////
	//  Accessor/Mutator

public:

	FVitalsStruct GetPlayerVitals();




};

