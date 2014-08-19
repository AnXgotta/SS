// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
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

UCLASS(config=Game)
class ASSCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	TSubobjectPtr<class USkeletalMeshComponent> Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class UCameraComponent> FirstPersonCameraComponent;


protected:

	///////////////////////////////////////////////////////
	//  Player Vitals

	UPROPERTY(Replicated)
	FVitalsStruct PlayerVitals;



	//////////////////////////////////////////////////////
	//  Player Movement

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	// Handles sprint start
	void SprintStart();

	// Handles sprint end
	void SprintEnd();

private:
	UPROPERTY(EditDefaultsOnly, Category = Sprint)
		float SprintMultiplier;

	float DefaultWalkSpeed;



	///////////////////////////////////////////////////
	//  Player Input Management

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface



};

