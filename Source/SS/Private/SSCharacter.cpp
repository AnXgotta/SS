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
	Mesh1P->SetOnlyOwnerSee(false);
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->RelativeLocation = FVector(0.f, 0.f, -150.f);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	// replication
	bReplicateMovement = true;
	bReplicates = true;




	
	


	

}



////////////////////////////////////////////////////////////////////////
//  Player Initialization

void ASSCharacter::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	CrouchImplementation(DeltaSeconds);

}

void ASSCharacter::PostInitializeComponents(){
	Super::PostInitializeComponents();
	InitializePlayer();
	InitializeInventory();
}

void ASSCharacter::InitializePlayer(){

	CrouchInterpValue = 0.0f;
	CachedCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	
	// character movement setup	
	if (CharacterMovement){
		CharacterMovement->MaxStepHeight = 45.0f;
		CharacterMovement->JumpZVelocity = JUMP_VELOCITY * 100.0f;
		CharacterMovement->SetWalkableFloorAngle(45.0f);
		CharacterMovement->GravityScale = 1.0;
		CharacterMovement->GroundFriction = 8.0f;
		CharacterMovement->MaxWalkSpeed = MAX_WALK_SPEED * 100.0f;
		CharacterMovement->MaxWalkSpeedCrouched = MAX_WALK_SPEED * CROUCH_SPEED_MULTIPLIER;
		CharacterMovement->MaxCustomMovementSpeed = 600.0f;
		CharacterMovement->MaxSwimSpeed = 300.0f;
		CharacterMovement->MaxAcceleration = 2048.0f;
		CharacterMovement->BrakingDecelerationWalking = 2048.0f;
		CharacterMovement->BrakingDecelerationFalling = 0.0f;
		CharacterMovement->BrakingDecelerationSwimming = 0.0f;
		CharacterMovement->AirControl = 0.05f;
		CharacterMovement->FallingLateralFriction = 0.0f;
		CharacterMovement->CrouchedHalfHeight = 40.0f;
		CharacterMovement->Buoyancy = 0.0f;
		CharacterMovement->bOrientRotationToMovement = false;
		CharacterMovement->bCanWalkOffLedges = true;
		CharacterMovement->bCanWalkOffLedgesWhenCrouching = true;
		CharacterMovement->Mass = 100.0f;
		CharacterMovement->bMaintainHorizontalGroundVelocity = true;
		CharacterMovement->bImpartBaseVelocityX = true;
		CharacterMovement->bImpartBaseVelocityY = true;
		CharacterMovement->bImpartBaseVelocityZ = true;
		CharacterMovement->bImpartBaseAngularVelocity = true;
		CharacterMovement->bIgnoreBaseRotation = false;
		CharacterMovement->bAlwaysCheckFloor = true;
		CharacterMovement->JumpOffJumpZFactor = 0.5f;
		CharacterMovement->PerchRadiusThreshold = 0.0f;
		CharacterMovement->PerchAdditionalHeight = 40.0f;
		CharacterMovement->bUseControllerDesiredRotation = false;
		CharacterMovement->bEnableScopedMovementUpdates = true;
		CharacterMovement->bUseFlatBaseForFloorChecks = false;
		CharacterMovement->bRequestedMoveUseAcceleration = true;
		// 4.5 CONVERSION CharacterMovement->bCrouchMovesCharacterDown = true;
		
	}
	// sprint setup
	SprintMultiplier = 1.5f;
	CachedDefaultWalkSpeed = CharacterMovement->MaxWalkSpeed;

}

void ASSCharacter::InitializeInventory(){
	if (Role == ROLE_Authority){
		for (int32 i = 0; i < 32; i++){
//			PlayerInventory.Add(NewObject<USSInventorySlot>());
		}
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
	if (Role != ROLE_Authority){
		ServerSprintStart();
		return;
	}
	if (CharacterMovement){
		CharacterMovement->MaxWalkSpeed = CachedDefaultWalkSpeed * SprintMultiplier;
	}	
}

bool ASSCharacter::ServerSprintStart_Validate(){
	return true;
}

void ASSCharacter::ServerSprintStart_Implementation(){
	SprintStart();
}

void ASSCharacter::SprintEnd(){
	if (Role != ROLE_Authority){
		ServerSprintEnd();
		return;
	}
	if (CharacterMovement){
		CharacterMovement->MaxWalkSpeed = CachedDefaultWalkSpeed;
	}	
}

bool ASSCharacter::ServerSprintEnd_Validate(){
	return true;
}

void ASSCharacter::ServerSprintEnd_Implementation(){
	SprintEnd();
}

void ASSCharacter::CrouchStart(){	
	if (Role != ROLE_Authority){
		ServerCrouchStart();		
		return;
	}

	bIsCrouching = true;
	bIsPlayerCrouched = false;
}

bool ASSCharacter::ServerCrouchStart_Validate(){
	return true;
}

void ASSCharacter::ServerCrouchStart_Implementation(){
	CrouchStart();
}

void ASSCharacter::CrouchEnd(){
	if (Role != ROLE_Authority){
		ServerCrouchEnd();		
		return;
	}

	bIsCrouching = false;
	bIsPlayerCrouched = true;
}

bool ASSCharacter::ServerCrouchEnd_Validate(){
	return true;
}

void ASSCharacter::ServerCrouchEnd_Implementation(){
	CrouchEnd();
}

void ASSCharacter::CrouchImplementation(float DeltaSeconds){
	if (bIsCrouching){
		if (bIsPlayerCrouched) return;

		if (CrouchInterpValue >= 1.0f){
			CrouchInterpValue = 1.0f;
			bIsPlayerCrouched = true;
			return;
		}

		if (CharacterMovement && CharacterMovement->IsMovingOnGround()){			
			CrouchInterpValue = FMath::Clamp(CrouchInterpValue + DeltaSeconds * 5.0f, 0.0f, 1.0f);
			// new dimensions
			float const NewHalfHeight = CachedCapsuleHalfHeight - (CrouchInterpValue * (CachedCapsuleHalfHeight - (CachedCapsuleHalfHeight - CrouchedEyeHeight)));
			CapsuleComponent->SetCapsuleSize(CapsuleComponent->GetScaledCapsuleRadius(), NewHalfHeight, true);			
		}
	}else{
		if (!bIsPlayerCrouched) return;

		if (CrouchInterpValue <= 0.0f){
			CrouchInterpValue = 0.0f;
			bIsPlayerCrouched = false;
			return;
		}

		//  used to check if player is trying to stand while under something
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Crouch_Trace")), true, this);
		FHitResult HitResult(ForceInit);
		const FVector StartPos = GetActorLocation();
		const FVector EndPos = StartPos + CapsuleComponent->GetUpVector() * CapsuleComponent->GetScaledCapsuleHalfHeight() + 5.0f;
		// return if player under object
		if (GetWorld()->LineTraceSingle(HitResult, StartPos, EndPos, TraceParams, NULL)){
			return;
		}		

		if (CharacterMovement && CharacterMovement->IsMovingOnGround()){
			CrouchInterpValue = FMath::Clamp(CrouchInterpValue - DeltaSeconds * 5.0f, 0.0f, 1.0f);
			// new dimensions
			float const NewHalfHeight = CachedCapsuleHalfHeight - (CrouchInterpValue * (CachedCapsuleHalfHeight - (CachedCapsuleHalfHeight - CrouchedEyeHeight)));
			CapsuleComponent->SetCapsuleSize(CapsuleComponent->GetScaledCapsuleRadius(), NewHalfHeight, true);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Player Input Management

void ASSCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("LeftShift", IE_Pressed, this, &ASSCharacter::SprintStart);
	InputComponent->BindAction("LeftShift", IE_Released, this, &ASSCharacter::SprintEnd);
	InputComponent->BindAction("LeftCtrl", IE_Pressed, this, &ASSCharacter::CrouchStart);
	InputComponent->BindAction("LeftCtrl", IE_Released, this, &ASSCharacter::CrouchEnd);

	InputComponent->BindAxis("MoveForward", this, &ASSCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASSCharacter::MoveRight);

	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}


/////////////////////////////////////////////////////////////////////////
//  Player Replicated Props

void ASSCharacter::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASSCharacter, PlayerVitals, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(ASSCharacter, PlayerClothes, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASSCharacter, PlayerBelt, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASSCharacter, PlayerPack, COND_OwnerOnly);

	DOREPLIFETIME(ASSCharacter, bIsCrouching);
	DOREPLIFETIME(ASSCharacter, bIsPlayerCrouched);

}

///////////////////////////////////////////////////////
//  Accessor/Mutator

FVitalsStruct ASSCharacter::GetPlayerVitals(){
	return PlayerVitals;
}
