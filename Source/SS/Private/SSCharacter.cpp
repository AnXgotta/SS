// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "SS.h"
#include "SSCharacter.h"
#include "SSProjectile.h"
#include "Animation/AnimInstance.h"


//////////////////////////////////////////////////////////////////////////
// ASSCharacter Constructor

ASSCharacter::ASSCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP){

	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = CapsuleComponent;
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->RelativeLocation = FVector(0.f, 0.f, -150.f);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)





	CachedDefaultWalkSpeed = CharacterMovement->MaxWalkSpeed;



	for (int32 i = 0; i < 32; i++){
		PlayerInventory.Add(FInventorySlotStruct());
	}

}



////////////////////////////////////////////////////////////////////////
//  Player Movement
void ASSCharacter::MoveForward(float Value){	
	if (Value != 0.0f){
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASSCharacter::MoveRight(float Value){
	if (Value != 0.0f){
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASSCharacter::SprintStart(){
	CharacterMovement->MaxWalkSpeed = CachedDefaultWalkSpeed * SprintMultiplier;
}

void ASSCharacter::SprintEnd(){
	CharacterMovement->MaxWalkSpeed = CachedDefaultWalkSpeed;
}



//////////////////////////////////////////////////////////////////////////
// Player Input Management

void ASSCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("LeftShift", IE_Pressed, this, &ASSCharacter::SprintStart);
	InputComponent->BindAction("LeftShift", IE_Released, this, &ASSCharacter::SprintEnd);

	InputComponent->BindAxis("MoveForward", this, &ASSCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASSCharacter::MoveRight);

	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}


/////////////////////////////////////////////////////////////////////////
//  Player Replicated Props

void ASSCharacter::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASSCharacter, PlayerVitals);
	DOREPLIFETIME(ASSCharacter, PlayerInventory);

}

///////////////////////////////////////////////////////
//  Accessor/Mutator

FVitalsStruct ASSCharacter::GetPlayerVitals(){
	return PlayerVitals;
}
